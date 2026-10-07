#include "Rendering/Device.h"

HRESULT
Device::CreateRenderTargetView(ID3D11Resource* pResource,
								const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
								ID3D11RenderTargetView** ppRTView){
	if(ppRTView == nullptr){
		ERROR("DEVICE", "CreateRenderTargetView", "Output pointer is null.");
		return E_POINTER;
	}

	*ppRTView = nullptr;

	if (m_device == nullptr || pResource == nullptr) {
		ERROR("DEVICE", "CreateRenderTargetView", "Device or resource pointer is null.");
		return E_INVALIDARG;
	}

	HRESULT result = m_device->CreateRenderTargetView(pResource, pDesc, ppRTView);
	if (FAILED(result)) {
		ERROR("Device",
			"CreateRenderTargetView",
			("Failed to create render target view. HRESULT: " + std::to_string(result)).c_str());
		return result;
	}

	MESSAGE("Device", "CreateRenderTargetView", "SUCCESS");
	return result;
}

