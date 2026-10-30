#include "CubismCompositor_CC.h"
#include "CubismRenderer_CC.h"
#include "CubismBlendShaders_CC.h"
#include "Model/CubismModel.hpp"
#include "renderer/backend/gfx/DeviceGFX.h"
#include "renderer/backend/RenderTarget.h"
#include <algorithm>
#include <map>
#include <memory>

using namespace cocos2d;
using namespace cocos2d::backend;
using namespace Live2D::Cubism::Framework;
using namespace Live2D::Cubism::Framework::Rendering;

namespace {
using Color = CubismRenderer::CubismTextureColor;
using Gfx = DeviceGFX;

const char* VertexShader = R"(
layout(location=0) in vec2 a_position;
layout(location=1) in vec2 a_texCoord;
layout(std140,binding=0) uniform VSBlock { mat4 u_matrix; };
layout(location=0) out vec2 v_texCoord;
void main() {
    gl_Position = u_matrix * vec4(a_position, 0.0, 1.0);
    v_texCoord = a_texCoord;
}
)";

const char* FragmentInterface = R"(
layout(location=0) in vec2 v_texCoord;
layout(std140,binding=1) uniform FSBlock {
    vec4 u_baseColor;
    vec4 u_multiplyColor;
    vec4 u_screenColor;
    vec4 u_params;
};
layout(binding=2) uniform sampler2D s_texture0;
layout(binding=3) uniform sampler2D s_texture1;
layout(binding=4) uniform sampler2D s_texture2;
layout(location=0) out vec4 cc_FragColor;
)";

const char* MeshFragment = R"(
void main() {
    vec4 color = texture(s_texture0, vec2(v_texCoord.x, 1.0 - v_texCoord.y));
    if (u_params.x > 0.5) {
        // Mask coverage ignores drawable opacity and color, like the SDK mask shader.
        cc_FragColor = vec4(color.a);
        return;
    }
    if (u_params.y > 0.5)
        color.rgb = color.a > 0.00001 ? color.rgb / color.a : vec3(0.0);
    color.rgb *= u_multiplyColor.rgb;
    color.rgb += u_screenColor.rgb - color.rgb * u_screenColor.rgb;
    color *= u_baseColor;
    color.rgb *= color.a;
    cc_FragColor = color;
}
)";

const char* CompositeFragment = R"(
void main() {
    vec4 source = ConvertPremultipliedToStraight(texture(s_texture0, v_texCoord));
    vec4 destinationPma = texture(s_texture1, v_texCoord);
    vec4 destination = ConvertPremultipliedToStraight(destinationPma);
    source.rgb *= u_multiplyColor.rgb;
    source.rgb += u_screenColor.rgb - source.rgb * u_screenColor.rgb;
    source *= u_baseColor;
    if (u_params.x > 0.5) {
        float mask = texture(s_texture2, v_texCoord).a;
        source.a *= u_params.y > 0.5 ? 1.0 - mask : mask;
    }
#if CSM_LEGACY_BLEND == 1
    cc_FragColor = vec4(source.rgb * source.a + destinationPma.rgb, destination.a);
#elif CSM_LEGACY_BLEND == 2
    cc_FragColor = vec4(destinationPma.rgb * (source.rgb * source.a + 1.0 - source.a), destination.a);
#else
    cc_FragColor = AlphaBlend(ColorBlend(source.rgb, destination.rgb), source, destination);
#endif
}
)";

Program* CreateProgram(const std::string& name, const std::string& fragment)
{
    Gfx::ShaderInfo info;
    info.name = name;
    info.attributes = {
        {"a_position", VertexFormat::FLOAT2, false, 0, false, 0},
        {"a_texCoord", VertexFormat::FLOAT2, false, 0, false, 1}
    };
    info.stages = {
        {Gfx::ShaderStageFlagBit::VERTEX, VertexShader},
        {Gfx::ShaderStageFlagBit::FRAGMENT, std::string(FragmentInterface) + fragment}
    };
    info.blocks = {
        {0, 0, "VSBlock", {{"u_matrix", Gfx::Type::MAT4, 1}}, 1},
        {0, 1, "FSBlock", {
            {"u_baseColor", Gfx::Type::FLOAT4, 1},
            {"u_multiplyColor", Gfx::Type::FLOAT4, 1},
            {"u_screenColor", Gfx::Type::FLOAT4, 1},
            {"u_params", Gfx::Type::FLOAT4, 1}}, 1}
    };
    info.samplerTextures = {
        {0, 2, "s_texture0", Gfx::Type::SAMPLER2D, 1},
        {0, 3, "s_texture1", Gfx::Type::SAMPLER2D, 1},
        {0, 4, "s_texture2", Gfx::Type::SAMPLER2D, 1}
    };
    return static_cast<Gfx*>(backend::Device::getInstance())->newProgram(info);
}

// Raw render targets avoid RenderTexture's content-scale conversion and its
// immediate clear/render during construction. Sizes here are physical pixels.
struct Surface
{
    Texture2D* texture = nullptr;
    RenderTarget* target = nullptr;
    unsigned width = 0, height = 0;

    ~Surface() { CC_SAFE_RELEASE(target); CC_SAFE_RELEASE(texture); }
    void Resize(unsigned w, unsigned h)
    {
        if (width == w && height == h) return;
        CC_SAFE_RELEASE_NULL(target);
        CC_SAFE_RELEASE_NULL(texture);
        TextureDescriptor descriptor;
        descriptor.width = w;
        descriptor.height = h;
        descriptor.textureUsage = TextureUsage::RENDER_TARGET;
        descriptor.textureFormat = PixelFormat::RGBA8;
        texture = new Texture2D();
        texture->updateTextureDescriptor(descriptor, true);
        texture->setTexParameters({SamplerFilter::LINEAR, SamplerFilter::LINEAR,
            SamplerAddressMode::CLAMP_TO_EDGE, SamplerAddressMode::CLAMP_TO_EDGE});
        target = backend::Device::getInstance()->newRenderTarget(
            TargetBufferFlags::COLOR, texture->getBackendTexture());
        width = w;
        height = h;
    }
};

struct CompositeLayer
{
    Surface buffers[2];
    int current = 0;
    void Resize(unsigned w, unsigned h) { for (auto& b : buffers) b.Resize(w, h); }
    Surface& Current() { return buffers[current]; }
    Surface& Next() { return buffers[1 - current]; }
};
}

struct CubismCompositor_CC::Impl
{
    CubismRenderer_CC* owner;
    Renderer* renderer;
    CubismModel* model;
    Program* meshProgram;
    Program* copyProgram;
    std::map<int, Program*> blendPrograms;
    CompositeLayer root;
    // Sibling offscreens reuse a layer once their contents have been composited.
    std::vector<std::unique_ptr<CompositeLayer>> layers;
    Surface source, mask;
    std::vector<int> stack;
    std::vector<std::unique_ptr<CubismDrawCommand_CC>> commands;
    RenderTarget* savedTarget = nullptr;
    Viewport savedViewport;
    unsigned width = 0, height = 0;

    explicit Impl(CubismRenderer_CC* value)
        : owner(value), renderer(value->ccr), model(value->GetModel())
    {
        meshProgram = CreateProgram("Cubism5Mesh", MeshFragment);
        copyProgram = CreateProgram("Cubism5Copy", R"(
void main() { cc_FragColor = texture(s_texture0, v_texCoord) * u_baseColor; }
)");
        for (int i = 0; i < model->GetDrawableCount(); ++i)
            BlendProgram(model->GetDrawableBlendModeType(i));
        for (int i = 0; i < model->GetOffscreenCount(); ++i)
        {
            BlendProgram(model->GetOffscreenBlendModeType(i));
        }
    }

    ~Impl()
    {
        commands.clear();
        for (auto& p : blendPrograms) CC_SAFE_RELEASE(p.second);
        CC_SAFE_RELEASE(meshProgram);
        CC_SAFE_RELEASE(copyProgram);
    }

    Program* BlendProgram(const csmBlendMode& mode)
    {
        const int color = mode.GetColorBlendType(), alpha = mode.GetAlphaBlendType();
        const int key = color | (alpha << 8);
        const auto found = blendPrograms.find(key);
        if (found != blendPrograms.end()) return found->second;
        // Core reserves 1/2 for the old additive/multiplicative equations.
        const int legacy = alpha == 0 && (color == 1 || color == 2) ? color : 0;
        const int shaderColor = color >= 3 ? color - 2 : 0;
        std::string shader = "#define CSM_COLOR_BLEND_MODE " + std::to_string(shaderColor)
            + "\n#define CSM_ALPHA_BLEND_MODE " + std::to_string(alpha)
            + "\n#define CSM_LEGACY_BLEND " + std::to_string(legacy) + "\n";
        shader += CubismColorBlendShader_CC;
        shader += CubismAlphaBlendShader_CC;
        shader += CompositeFragment;
        auto program = CreateProgram("Cubism5Blend" + std::to_string(key), shader);
        blendPrograms.emplace(key, program);
        return program;
    }

    void Bind(Surface& surface, bool clear = false)
    {
        auto target = surface.target;
        owner->AddCallBack([this, target]() {
            renderer->setRenderTarget(target);
            renderer->setViewPort(0, 0, width, height);
        });
        if (clear) renderer->clear(ClearFlag::COLOR, Color4F(0, 0, 0, 0), 1, 0, 0);
    }

    void Uniform(ProgramState* state, Program* program, const char* name, const void* data, size_t size)
    {
        auto location = program->getUniformLocation(name);
        if (location) state->setUniform(location, data, size);
    }

    void DrawMesh(Program* program, Texture2D* texture, Texture2D* destination, Texture2D* maskTexture,
        const csmFloat32* vertices, const csmFloat32* uvs, int vertexCount,
        const csmUint16* indices, int indexCount, CubismMatrix44 matrix,
        const Color& base, const Color& multiply, const Color& screen, const Vec4& params, bool blend)
    {
        if (!program || !texture || vertexCount <= 0 || indexCount <= 0) return;
        std::unique_ptr<CubismDrawCommand_CC> command(new CubismDrawCommand_CC());
        command->CreateVertexBuffer(sizeof(float) * 4, vertexCount);
        command->CreateIndexBuffer(indexCount);
        command->UpdateVertexBuffer(const_cast<float*>(vertices), const_cast<float*>(uvs), vertexCount);
        command->UpdateIndexBuffer(const_cast<csmUint16*>(indices), indexCount);
        auto cmd = command->GetCommand();
        auto& desc = cmd->getPipelineDescriptor();
        auto state = new ProgramState(program);
        desc.programState = state;
        state->setTexture(program->getUniformLocation("s_texture0"), 0, texture->getBackendTexture());
        auto bindTexture = [&](const char* name, int unit, Texture2D* tex) {
            auto location = program->getUniformLocation(name);
            if (location) state->setTexture(location, unit, (tex ? tex : texture)->getBackendTexture());
        };
        bindTexture("s_texture1", 1, destination);
        bindTexture("s_texture2", 2, maskTexture);
        Uniform(state, program, "u_matrix", matrix.GetArray(), sizeof(float) * 16);
        const Vec4 b(base.R, base.G, base.B, base.A), m(multiply.R, multiply.G, multiply.B, multiply.A);
        const Vec4 s(screen.R, screen.G, screen.B, screen.A);
        Uniform(state, program, "u_baseColor", &b, sizeof(b));
        Uniform(state, program, "u_multiplyColor", &m, sizeof(m));
        Uniform(state, program, "u_screenColor", &s, sizeof(s));
        Uniform(state, program, "u_params", &params, sizeof(params));
        desc.blendDescriptor.blendEnabled = blend;
        desc.blendDescriptor.sourceRGBBlendFactor = BlendFactor::ONE;
        desc.blendDescriptor.destinationRGBBlendFactor = BlendFactor::ONE_MINUS_SRC_ALPHA;
        desc.blendDescriptor.sourceAlphaBlendFactor = BlendFactor::ONE;
        desc.blendDescriptor.destinationAlphaBlendFactor = BlendFactor::ONE_MINUS_SRC_ALPHA;
        renderer->addCommand(cmd);
        commands.emplace_back(std::move(command));
    }

    void Quad(Program* program, Texture2D* texture, Texture2D* destination, Texture2D* maskTexture,
        const Color& base, const Color& multiply, const Color& screen, const Vec4& params, bool blend = false)
    {
        const float positions[] = {-1,-1, 1,-1, 1,1, -1,1};
        const auto api = Gfx::getAPI();
        const bool flip = api == Gfx::VULKAN || api == Gfx::METAL;
        const float bottom = flip ? 1.0f : 0.0f, top = 1.0f - bottom;
        const float uvs[] = {0,bottom, 1,bottom, 1,top, 0,top};
        const csmUint16 indices[] = {0,1,2, 0,2,3};
        CubismMatrix44 identity;
        identity.LoadIdentity();
        owner->SetCullMode(CullMode::NONE);
        DrawMesh(program, texture, destination, maskTexture, positions, uvs, 4, indices, 6,
            identity, base, multiply, screen, params, blend);
    }

    void Mesh(int index, bool isMask)
    {
        auto texture = owner->_textures.find(model->GetDrawableTextureIndex(index));
        if (texture == owner->_textures.end()) return;
        owner->SetCullMode(model->GetDrawableCulling(index) ? CullMode::BACK : CullMode::NONE);
        auto& colors = model->GetOverrideMultiplyAndScreenColor();
        DrawMesh(meshProgram, texture->second, nullptr, nullptr,
            model->GetDrawableVertices(index), reinterpret_cast<const float*>(model->GetDrawableVertexUvs(index)),
            model->GetDrawableVertexCount(index), model->GetDrawableVertexIndices(index),
            model->GetDrawableVertexIndexCount(index), owner->GetMvpMatrix(),
            Color(1, 1, 1, isMask ? 1.0f : model->GetDrawableOpacity(index)),
            colors.GetDrawableMultiplyColor(index), colors.GetDrawableScreenColor(index),
            Vec4(isMask ? 1.0f : 0.0f, owner->IsPremultipliedAlpha() ? 1.0f : 0.0f, 0, 0), true);
    }

    Texture2D* Mask(const csmInt32* indices, int count)
    {
        if (count <= 0) return nullptr;
        Bind(mask, true);
        for (int i = 0; i < count; ++i) Mesh(indices[i], true);
        return mask.texture;
    }

    CompositeLayer& Current() { return stack.empty() ? root : *layers[stack.size() - 1]; }

    void Composite(CompositeLayer& layer, Texture2D* texture, const csmBlendMode& mode,
        Texture2D* maskTexture, bool inverted, float opacity, const Color& multiply, const Color& screen)
    {
        auto destination = layer.Current().texture;
        Bind(layer.Next());
        Quad(BlendProgram(mode), texture, destination, maskTexture, Color(1, 1, 1, opacity),
            multiply, screen, Vec4(maskTexture ? 1.0f : 0.0f, inverted ? 1.0f : 0.0f, 0, 0));
        layer.current = 1 - layer.current;
    }

    bool IsDescendant(int part, int ownerPart) const
    {
        while (part != CubismModel::CubismNoIndex_Parent)
        {
            if (part == ownerPart) return true;
            part = model->GetPartParentPartIndex(part);
        }
        return false;
    }

    void FinishOffscreen()
    {
        const int index = stack.back();
        auto texture = Current().Current().texture;
        stack.pop_back();
        auto maskTexture = Mask(model->GetOffscreenMasks()[index], model->GetOffscreenMaskCounts()[index]);
        auto& colors = model->GetOverrideMultiplyAndScreenColor();
        Composite(Current(), texture, model->GetOffscreenBlendModeType(index), maskTexture,
            model->GetOffscreenInvertedMask(index), model->GetOffscreenOpacity(index),
            colors.GetOffscreenMultiplyColor(index), colors.GetOffscreenScreenColor(index));
    }

    void Draw()
    {
        commands.clear();
        stack.clear();
        // The camera viewport is in physical pixels, including Retina/high-DPI output.
        const auto viewport = renderer->getViewport();
        width = std::max(1u, static_cast<unsigned>(viewport.w));
        height = std::max(1u, static_cast<unsigned>(viewport.h));
        owner->SetRenderTargetSize(width, height);
        root.Resize(width, height);
        source.Resize(width, height);
        mask.Resize(width, height);
        for (auto& layer : layers) layer->Resize(width, height);
        owner->AddCallBack([this]() {
            savedTarget = renderer->getRenderTarget();
            savedViewport = renderer->getViewport();
        });
        owner->PreDraw();
        root.current = 0;
        Bind(root.Current(), true);
        const int drawableCount = model->GetDrawableCount();
        const int total = drawableCount + model->GetOffscreenCount();
        std::vector<int> objects(total);
        for (int i = 0; i < total; ++i) objects[i] = i;
        const auto orders = model->GetRenderOrders();
        std::stable_sort(objects.begin(), objects.end(), [orders](int a, int b) { return orders[a] < orders[b]; });
        for (int object : objects)
        {
            const bool offscreen = object >= drawableCount;
            const int index = offscreen ? object - drawableCount : object;
            const int part = offscreen
                ? model->GetPartParentPartIndex(model->GetOffscreenOwnerIndices()[index])
                : model->GetDrawableParentPartIndex(index);
            while (!stack.empty() && !IsDescendant(part, model->GetOffscreenOwnerIndices()[stack.back()]))
                FinishOffscreen();
            if (offscreen)
            {
                stack.push_back(index);
                if (layers.size() < stack.size())
                {
                    layers.emplace_back(new CompositeLayer());
                    layers.back()->Resize(width, height);
                }
                auto& layer = Current();
                layer.current = 0;
                Bind(layer.Current(), true);
                continue;
            }
            if (!model->GetDrawableDynamicFlagIsVisible(index)) continue;
            auto maskTexture = Mask(model->GetDrawableMasks()[index], model->GetDrawableMaskCounts()[index]);
            Bind(source, true);
            Mesh(index, false);
            Composite(Current(), source.texture, model->GetDrawableBlendModeType(index), maskTexture,
                model->GetDrawableInvertedMask(index), 1.0f, Color(), Color(0, 0, 0, 0));
        }
        while (!stack.empty()) FinishOffscreen();
        owner->AddCallBack([this]() {
            renderer->setRenderTarget(savedTarget);
            renderer->setViewPort(savedViewport.x, savedViewport.y, savedViewport.w, savedViewport.h);
        });
        auto color = owner->GetModelColor();
        color.R *= color.A;
        color.G *= color.A;
        color.B *= color.A;
        Quad(copyProgram, root.Current().texture, nullptr, nullptr, color, Color(), Color(0,0,0,0), Vec4::ZERO, true);
    }
};

CubismCompositor_CC::CubismCompositor_CC(CubismRenderer_CC* renderer) : _impl(new Impl(renderer)) {}
CubismCompositor_CC::~CubismCompositor_CC() { delete _impl; }
void CubismCompositor_CC::Draw() { _impl->Draw(); }
