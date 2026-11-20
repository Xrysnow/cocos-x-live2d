#pragma once
#include "Type/csmMap.hpp"
#include "2d/RenderTexture.h"

namespace Live2D { namespace Cubism { namespace Framework { namespace Rendering {
	class CubismRenderer_CC;

	class CubismOffscreenFrame_CC
	{
		friend class CubismRenderer_CC;
	public:
		CubismOffscreenFrame_CC();

		void BeginDraw();

		void EndDraw();

		void Clear(float r, float g, float b, float a);

		csmBool CreateOffscreenFrame(csmUint32 displayBufferWidth, csmUint32 displayBufferHeight, cc::RenderTexture* renderTexture = nullptr);

		void DestroyOffscreenFrame();

		cc::Texture2D* GetColorBuffer() const;

		csmUint32 GetBufferWidth() const;

		csmUint32 GetBufferHeight() const;

		cc::Size GetViewPortSize() const { return _viewPortSize; }

		csmBool IsValid() const;

		cc::RenderTexture* GetOffscreenFrame() const;

	private:
		CubismRenderer_CC* _renderer = nullptr;
		cc::Renderer* ccr = nullptr;
#ifdef CC_VERSION
		cc::backend::RenderTarget* _oldRenderTarget = nullptr;
		cc::backend::RenderTarget* _renderTarget = nullptr;
#else
		cc::Texture2D* _oldColorAttachment = nullptr;
		cc::Texture2D* _oldDepthAttachment = nullptr;
		cc::Texture2D* _oldStencilAttachment = nullptr;
		cc::RenderTargetFlag _oldRenderTargetFlag = {};
#endif // CC_VERSION

		cc::RenderTexture*  _renderTexture = nullptr;         ///< レンダリングターゲットとしてのアドレス
		cc::Texture2D*      _colorBuffer = nullptr;           ///< 描画の際使用するテクスチャとしてのアドレス
		csmUint32   _bufferWidth = 0;           ///< Create時に指定された幅
		csmUint32   _bufferHeight = 0;          ///< Create時に指定された高さ
		bool _isInheritedRenderTexture = false;
		cc::Texture2D* _previousColorBuffer = nullptr; ///< 旧フレームバッファ
		cc::Size _viewPortSize;
	};

}}}}
