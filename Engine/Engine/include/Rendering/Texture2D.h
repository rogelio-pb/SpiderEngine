#pragma once
#include "Engine/Prerequisites.h"

class Device;
class DeviceContext;

class
    Texture2D {
public:
    Texture2D() noexcept = default;
    ~Texture2D() noexcept;

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&&) = delete;
    Texture2D& operator=(Texture2D&&) = delete;

    HRESULT
        init(Device& device,
            const std::string& textureName,
            ExtensionType extensionType);

    HRESULT
        init(Device& device,
            unsigned int width,
            unsigned int height,
            DXGI_FORMAT Format,
            unsigned int BindFlags,
            unsigned int sampleCount = 1,
            unsigned int qualityLevels = 0);

    HRESULT
        init(Device& device, Texture2D& textureRef, DXGI_FORMAT format);

    HRESULT
        InitTextureFromImage(Device& device,
            const std::string& fullPath,
            Texture2D& texture);

    void
        render(DeviceContext& deviceContext, unsigned int StartSlot, unsigned int NumViews);


    void
        destroy();

    HRESULT
        CreateCubemap(Device& device,
            DeviceContext& deviceContext,
            const std::array<std::string, 6>& facePaths,
            bool generateMips /*= false*/);

    ID3D11ShaderResourceView*
        CreateCubemapFaceSRV(ID3D11Device* device,
            ID3D11Texture2D* cubemapTex,
            DXGI_FORMAT format,
            unsigned int faceIndex,
            unsigned int mipLevels = 1)
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC d{};
        d.Format = format;
        d.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
        d.Texture2DArray.MostDetailedMip = 0;
        d.Texture2DArray.MipLevels = mipLevels;       
        d.Texture2DArray.FirstArraySlice = faceIndex; 
        d.Texture2DArray.ArraySize = 1;               

        ID3D11ShaderResourceView* srv = nullptr;
        if (FAILED(device->CreateShaderResourceView(cubemapTex, &d, &srv)))
            return nullptr;

        return srv;
    }


    ID3D11Texture2D*
        GetTexture() const noexcept {
        return m_texture;
    }

    ID3D11Texture2D**
        GetTextureAddress() noexcept {
        return &m_texture;
    }


public:

    ID3D11Texture2D* m_texture = nullptr;
    ID3D11ShaderResourceView* m_textureFromImg = nullptr;
    std::string m_textureName;
};

