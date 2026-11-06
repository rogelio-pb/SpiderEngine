#pragma once
#include "Engine/Prerequisites.h"
#include "Engine/MeshComponent.h"

class Device;
class DeviceContext;

class
    Buffer {
public:

    Buffer() = default;

    ~Buffer() = default;

    HRESULT
        init(Device& device, const MeshComponent& mesh, unsigned int bindFlag);

    HRESULT
        init(Device& device, unsigned int ByteWidth);

    void
        update(DeviceContext& deviceContext,
            ID3D11Resource* pDstResource,
            unsigned int    DstSubresource,
            const D3D11_BOX* pDstBox,
            const void* pSrcData,
            unsigned int    SrcRowPitch,
            unsigned int    SrcDepthPitch);

    void
        render(DeviceContext& deviceContext,
            unsigned int   StartSlot,
            unsigned int   NumBuffers,
            bool           setPixelShader = false,
            DXGI_FORMAT    format = DXGI_FORMAT_UNKNOWN);
    void
        destroy();

    HRESULT
        createBuffer(Device& device,
            D3D11_BUFFER_DESC& desc,
            D3D11_SUBRESOURCE_DATA* initData);

    ID3D11Buffer* getBuffer() const { return m_buffer; }

public:
    ID3D11Buffer* m_buffer = nullptr;
private:
    unsigned int m_stride = 0;
    unsigned int m_offset = 0;
    unsigned int m_bindFlag = 0;
};