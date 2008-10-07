// D3D10RenderFactory.cpp
// KlayGE D3D10��Ⱦ������󹤳� ʵ���ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2008
// Homepage: http://klayge.sourceforge.net
//
// 3.8.0
// ���ν��� (2008.9.21)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/Math.hpp>

#include <KlayGE/D3D10/D3D10RenderEngine.hpp>
#include <KlayGE/D3D10/D3D10Texture.hpp>
#include <KlayGE/D3D10/D3D10FrameBuffer.hpp>
#include <KlayGE/D3D10/D3D10RenderLayout.hpp>
#include <KlayGE/D3D10/D3D10GraphicsBuffer.hpp>
#include <KlayGE/D3D10/D3D10OcclusionQuery.hpp>
#include <KlayGE/D3D10/D3D10RenderView.hpp>
#include <KlayGE/D3D10/D3D10RenderStateObject.hpp>
#include <KlayGE/D3D10/D3D10ShaderObject.hpp>

#include <KlayGE/D3D10/D3D10RenderFactory.hpp>

namespace KlayGE
{
	D3D10RenderFactory::D3D10RenderFactory()
	{
	}

	std::wstring const & D3D10RenderFactory::Name() const
	{
		static std::wstring const name(L"Direct3D10 Render Factory");
		return name;
	}

	RenderEngine& D3D10RenderFactory::RenderEngineInstance()
	{
		static D3D10RenderEngine renderEngine;
		return renderEngine;
	}

	TexturePtr D3D10RenderFactory::MakeTexture1D(uint32_t width, uint16_t numMipMaps,
			ElementFormat format, uint32_t access_hint, ElementInitData* init_data)
	{
		return TexturePtr(new D3D10Texture1D(width, numMipMaps, format, access_hint, init_data));
	}
	TexturePtr D3D10RenderFactory::MakeTexture2D(uint32_t width, uint32_t height, uint16_t numMipMaps,
			ElementFormat format, uint32_t access_hint, ElementInitData* init_data)
	{
		return TexturePtr(new D3D10Texture2D(width, height, numMipMaps, format, access_hint, init_data));
	}
	TexturePtr D3D10RenderFactory::MakeTexture3D(uint32_t width, uint32_t height, uint32_t depth,
			uint16_t numMipMaps, ElementFormat format, uint32_t access_hint, ElementInitData* init_data)
	{
		return TexturePtr(new D3D10Texture3D(width, height, depth, numMipMaps, format, access_hint, init_data));
	}
	TexturePtr D3D10RenderFactory::MakeTextureCube(uint32_t size, uint16_t numMipMaps,
		ElementFormat format, uint32_t access_hint, ElementInitData* init_data)
	{
		return TexturePtr(new D3D10TextureCube(size, numMipMaps, format, access_hint, init_data));
	}

	FrameBufferPtr D3D10RenderFactory::MakeFrameBuffer()
	{
		return FrameBufferPtr(new D3D10FrameBuffer);
	}

	RenderLayoutPtr D3D10RenderFactory::MakeRenderLayout()
	{
		return RenderLayoutPtr(new D3D10RenderLayout);
	}

	GraphicsBufferPtr D3D10RenderFactory::MakeVertexBuffer(BufferUsage usage, uint32_t access_hint, ElementInitData* init_data)
	{
		return GraphicsBufferPtr(new D3D10GraphicsBuffer(usage, access_hint, D3D10_BIND_VERTEX_BUFFER, init_data));
	}

	GraphicsBufferPtr D3D10RenderFactory::MakeIndexBuffer(BufferUsage usage, uint32_t access_hint, ElementInitData* init_data)
	{
		return GraphicsBufferPtr(new D3D10GraphicsBuffer(usage, access_hint, D3D10_BIND_INDEX_BUFFER, init_data));
	}

	QueryPtr D3D10RenderFactory::MakeOcclusionQuery()
	{
		return QueryPtr(new D3D10OcclusionQuery);
	}

	RenderViewPtr D3D10RenderFactory::Make1DRenderView(Texture& texture, int level)
	{
		return RenderViewPtr(new D3D10RenderTargetRenderView(texture, level));
	}

	RenderViewPtr D3D10RenderFactory::Make2DRenderView(Texture& texture, int level)
	{
		return RenderViewPtr(new D3D10RenderTargetRenderView(texture, level));
	}

	RenderViewPtr D3D10RenderFactory::Make2DRenderView(Texture& texture, Texture::CubeFaces face, int level)
	{
		return RenderViewPtr(new D3D10RenderTargetRenderView(texture, face, level));
	}

	RenderViewPtr D3D10RenderFactory::Make3DRenderView(Texture& texture, uint32_t slice, int level)
	{
		return RenderViewPtr(new D3D10RenderTargetRenderView(texture, slice, level));
	}

	RenderViewPtr D3D10RenderFactory::MakeGraphicsBufferRenderView(GraphicsBuffer& gbuffer,
		uint32_t width, uint32_t height, ElementFormat pf)
	{
		return RenderViewPtr(new D3D10RenderTargetRenderView(gbuffer, width, height, pf));
	}

	RenderViewPtr D3D10RenderFactory::MakeDepthStencilRenderView(uint32_t width, uint32_t height,
		ElementFormat pf, uint32_t multi_sample)
	{
		return RenderViewPtr(new D3D10DepthStencilRenderView(width, height, pf, multi_sample));
	}

	ShaderObjectPtr D3D10RenderFactory::MakeShaderObject()
	{
		return ShaderObjectPtr(new D3D10ShaderObject);
	}

	RasterizerStateObjectPtr D3D10RenderFactory::DoMakeRasterizerStateObject(RasterizerStateDesc const & desc)
	{
		return RasterizerStateObjectPtr(new D3D10RasterizerStateObject(desc));
	}
	
	DepthStencilStateObjectPtr D3D10RenderFactory::DoMakeDepthStencilStateObject(DepthStencilStateDesc const & desc)
	{
		return DepthStencilStateObjectPtr(new D3D10DepthStencilStateObject(desc));
	}

	BlendStateObjectPtr D3D10RenderFactory::DoMakeBlendStateObject(BlendStateDesc const & desc)
	{
		return BlendStateObjectPtr(new D3D10BlendStateObject(desc));
	}

	SamplerStateObjectPtr D3D10RenderFactory::DoMakeSamplerStateObject(SamplerStateDesc const & desc)
	{
		return SamplerStateObjectPtr(new D3D10SamplerStateObject(desc));
	}

	RenderFactory& D3D10RenderFactoryInstance()
	{
		static D3D10RenderFactory renderFactory;
		return renderFactory;
	}
}