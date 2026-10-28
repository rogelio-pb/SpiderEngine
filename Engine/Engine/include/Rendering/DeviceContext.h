#pragma once
#include "Engine/Prerequisites.h"

class
	DeviceContext {
public:
	DeviceContext() = default;
	~DeviceContext();

	void
		init();

	void
		destroy();

	ID3D11DeviceContext*
		getDeviceContext() const noexcept
	{
		return m_deviceContext;
	}

	ID3D11DeviceContext**
		getDeviceContextAddress() noexcept
	{
		return &m_deviceContext;
	}

	ID3D11DeviceContext&
		requireContext() const noexcept;


	void
		ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
			const float colorRGBA[4]) noexcept;


	void
		ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
			unsigned int clearFlags,
			float depth,
			unsigned int stencil) noexcept;


	void
		OMSetRenderTargets(unsigned int numViews,
			ID3D11RenderTargetView* const* ppRenderTargetViews,
			ID3D11DepthStencilView* pDepthStencilView) noexcept;


	void
		RSSetViewports(unsigned int numViewports,
			const D3D11_VIEWPORT* pViewports) noexcept;


	void
		RSSetState(ID3D11RasterizerState* pRasterizerState) noexcept;


	void
		IASetInputLayout(ID3D11InputLayout* pInputLayout) noexcept;


	void
		IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology) noexcept;


	void
		IASetVertexBuffers(unsigned int startSlot,
			unsigned int numBuffers,
			ID3D11Buffer* const* ppVertexBuffers,
			const unsigned int* pStrides,
			const unsigned int* pOffsets) noexcept;


	void
		IASetIndexBuffer(ID3D11Buffer* pIndexBuffer,
			DXGI_FORMAT format,
			unsigned int offset) noexcept;


	void
		VSSetShader(ID3D11VertexShader* pVertexShader,
			ID3D11ClassInstance* const* ppClassInstances = nullptr,
			unsigned int numClassInstances = 0) noexcept;


	void
		PSSetShader(ID3D11PixelShader* pPixelShader,
			ID3D11ClassInstance* const* ppClassInstances = nullptr,
			unsigned int numClassInstances = 0) noexcept;


	void
		VSSetConstantBuffers(unsigned int startSlot,
			unsigned int numBuffers,
			ID3D11Buffer* const* ppConstantBuffers) noexcept;


	void
		UpdateSubresource(ID3D11Resource* pDstResource,
			unsigned int dstSubresource,
			const D3D11_BOX* pDstBox,
			const void* pSrcData,
			unsigned int srcRowPitch,
			unsigned int srcDepthPitch) noexcept;


	void
		DrawIndexed(unsigned int indexCount,
			unsigned int startIndexLocation = 0,
			int baseVertexLocation = 0) noexcept;


private:
	ID3D11DeviceContext* m_deviceContext = nullptr;
};