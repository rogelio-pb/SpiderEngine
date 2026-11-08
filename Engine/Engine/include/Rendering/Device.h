#pragma once
#include "Engine/Prerequisites.h"

/**
 * @class Device
 * @brief El de la tarjeta de video
 * Para dibujar en pantalla, la tarjeta de video necesita muchas piezas:
 * imágenes, cajas de datos, programas de dibujo, esta clase es la
 * encargada de fabricar todas esas pieza casi todo lo que se crea en
 * el motor pasa primero por aqui
 */
class
	Device {
public:
	Device() = default;
	~Device();

	/**
	* @brief Prepara el fabricante para empezar a trabajar
	*/
	void
		init();

	/**
	* @brief Libera el fabricante y todo el espacio que usaba
	* Hay que llamarla al cerrar el programa
	*/
	void
		destroy();

	/**
	* @brief Entrega el fabricante real de la tarjeta de video
	* @return El fabricante 
	*/
	ID3D11Device*
		getDevice() const noexcept
	{
		return m_device;
	}

	/**
	* @brief Entrega la "dirección" donde vive el fabricante
	* Se usa cuando otra función necesita guardar ahí al fabricante recién
	* creado, por ejemplo, al arrancar DirectX
	* @return La dirección donde se guarda el fabricante
	*/
	ID3D11Device**
		getDeviceAddress() noexcept
	{
		return &m_device;
	}

	/**
	* @brief Crea una sobre una imagen
	* Es la superficie donde la tarjeta pinta lo que luego se ve
	* en pantalla
	* @param pResource Imagen sobre la que se va a pintar
	* @param pDesc descripción de cómo se va a usar
	* @param ppRTView  Aquí se guarda la superficie creada
	* @return Un resultado que dice si todo salió bien o si hubo un error
	*/
	HRESULT
		CreateRenderTargetView(ID3D11Resource* pResource,
			const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
			ID3D11RenderTargetView** ppRTView);

	/**
	* @brief Crea una imagen en la memoria de la tarjeta de video
	* @param pDesc Características de la imagen, tamaño, formato, uso
	* @param pInitialData Contenido inicial de la imagen
	* @param ppTexture2D  Aquí se guarda la imagen creada
	* @return Un resultado que dice si todo salió bien o si hubo un error
	*/
	HRESULT
		CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
			const D3D11_SUBRESOURCE_DATA* pInitialData,
			ID3D11Texture2D** ppTexture2D);

	/**
 * @brief Crea la vista que guarda qué tan lejos o cerca está cada cosa
 * @param pResource Imagen donde se guarda esa informaciónde distancia
 * @param pDesc Descripción de cómo se va a usar
 * @param ppDepthStencilView  Aquí se guarda la vista creada
 * @return Un resultado que dice si todo salió bien o si hubo un error
 */
	HRESULT
		CreateDepthStencilView(ID3D11Resource* pResource,
			D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
			ID3D11DepthStencilView** ppDepthStencilView);
	HRESULT
		CreateVertexShader(const void* pShaderBytecode,
			unsigned int BytecodeLength,
			ID3D11ClassLinkage* pClassLinkage,
			ID3D11VertexShader** ppVertexShader);
	HRESULT
		CreatePixelShader(const void* pShaderBytecode,
			unsigned int BytecodeLength,
			ID3D11ClassLinkage* pClassLinkage,
			ID3D11PixelShader** ppPixelShader);
	HRESULT
		CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
			unsigned int NumElements,
			const void* pShaderBytecodeWithInputSignature,
			unsigned int BytecodeLength,
			ID3D11InputLayout** ppInputLayout);

	/**
	* @brief Crea una caja de datos en la memoria de la tarjeta de video
	* @param pInitialData Datos con los que se llena al inicio
	* @param ppBuffer     Aquí se guarda la caja creada
	* @return Un resultado que dice si todo salió bien o si hubo un error
	*/
	HRESULT
		CreateBuffer(const D3D11_BUFFER_DESC* pDesc,
			const D3D11_SUBRESOURCE_DATA* pInitialData,
			ID3D11Buffer** ppBuffer);
	HRESULT
		CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc,
			ID3D11RasterizerState** ppRasterizerState);
private:
	ID3D11Device* m_device = nullptr;
	D3D_FEATURE_LEVEL* m_featureLevels = nullptr;
};