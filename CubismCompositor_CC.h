#pragma once

namespace Live2D { namespace Cubism { namespace Framework { namespace Rendering {

class CubismRenderer_CC;

// Cubism 5.3 blend/offscreen rendering on the cocos gfx backend.
class CubismCompositor_CC
{
public:
    explicit CubismCompositor_CC(CubismRenderer_CC* renderer);
    ~CubismCompositor_CC();
    void Draw();

    CubismCompositor_CC(const CubismCompositor_CC&) = delete;
    CubismCompositor_CC& operator=(const CubismCompositor_CC&) = delete;

private:
    struct Impl;
    Impl* _impl;
};

}}}}
