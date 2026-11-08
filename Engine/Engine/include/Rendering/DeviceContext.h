#pragma once
#include "Engine/Prerequisites.h"

/**
* @class DeviceContext
* @brief Controla lo que contiene la tarjeta de video.
* 
* Si el Device fabrica las piezas, el DeviceContext es quien da las
* órdenes: qué pintar, con que colores, con qué programas y en qué
* orden. Cada vez que el motor dibuja algo en pantalla, le da una serie
* de instrucciones a esta clase
*/
class
DeviceContext {
public:
DeviceContext() = default;
~DeviceContext();

/**
* @brief Prepara al director para empezar a dar órdenes.
*/
void
	init();

/**
* @brief Libera al director y todo el espacio que usaba.
*
* Hay que llamarla al cerrar el programa.
*/
void
	destroy();

/**
 * @brief Libera al director y todo el espacio que usaba.
 *
 * Hay que llamarla al cerrar el programa.
 */
ID3D11DeviceContext*
	getDeviceContext() const noexcept
{
	return m_deviceContext;
}

/**
 * @brief Entrega la dirección donde vive el director
 * Se usa cuando otra función necesita guardar ahí al director
 * recién creado, por ejemplo, al arrancar DirectX
 * @return La dirección donde se guarda el director
 */
ID3D11DeviceContext**
	getDeviceContextAddress() noexcept
{
	return &m_deviceContext;
}

/**
 * @brief Entrega al director asegurándose de que ya exista
 * A diferencia de getDeviceContext(), esta función espera que el
 * director ya esté listo, así que no devuelve nulo
 * @return El director de la tarjeta de video
 */
ID3D11DeviceContext&
	requireContext() const noexcept;

/**
 * @brief Pinta toda la superficie de dibujo de un solo color
 * Es como limpiar el pizarrón antes de dibujar el siguiente cuadro
 * @param pRenderTargetView La superficie que se quiere limpiar.
 * @param colorRGBA El color de fondo: rojo, verde, azul y
 * transparencia, cada uno entre 0 y 1.
 */
void
	ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
		const float colorRGBA[4]) noexcept;

/**
 * @brief Borra la información de qué tan lejos está cada cosa
 * Se hace al inicio de cada cuadro para empezar desde cero
 * @param pDepthStencilView La vista de distancias que se limpia.
 * @param clearFlags: Qué se quiere borrar
 * @param depth: Valor de distancia con el que se rellena
 * @param stencil: Valor con el que se rellena la plantilla
 */
void
	ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
		unsigned int clearFlags,
		float depth,
		unsigned int stencil) noexcept;

/**
 * @brief Le dice a la tarjeta dónde debe dibujar
 * @param numViews: Cuántas superficies de dibujo se usan
 * @param ppRenderTargetViews: Las superficies donde se va a pintar
 * @param pDepthStencilView: La vista que lleva el control de
 * distancias
 */
void
	OMSetRenderTargets(unsigned int numViews,
		ID3D11RenderTargetView* const* ppRenderTargetViews,
		ID3D11DepthStencilView* pDepthStencilView) noexcept;

/**
 * @brief Define en qué parte de la pantalla se va a dibujar
 * Normalmente se usa toda la ventana, pero también puede ser solo
 * un pedazo
 * @param numViewports Cuántas zonas de dibujo se definen
 * @param pViewports   Las zonas: su posición y su tamaño
 */
void
	RSSetViewports(unsigned int numViewports,
		const D3D11_VIEWPORT* pViewports) noexcept;

/**
 * @brief Entrega imágenes al programa que pinta los colores
 * Así ese programa puede usar textura
 * @param StartSlot: Primer lugar donde se colocan las imágenes.
 * @param NumVie: cuántas imágenes se entregan.
 * @param ppShaderResourceViews: las imágenes que se entregan.
 */
void
	PSSetShaderResources(unsigned int StartSlot,
		unsigned int NumViews,
		ID3D11ShaderResourceView* const* ppShaderResourceViews);

void
	RSSetState(ID3D11RasterizerState* pRasterizerState) noexcept;

void
	IASetInputLayout(ID3D11InputLayout* pInputLayout) noexcept;

void
	IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology) noexcept;

/**
 * @brief Entrega a la tarjeta las cajas con los puntos de la figura
 * @param startSlot: primer lugar donde se colocan las cajas
 * @param numBuffers:cuantas cajas se entregan
 * @param ppVertexBuffers: Las cajas con los puntos
 * @param pOffsets: Cuántos bytes saltar al inicio de cada caja
 * antes de empezar a leer
 */
void
	IASetVertexBuffers(unsigned int startSlot,
		unsigned int numBuffers,
		ID3D11Buffer* const* ppVertexBuffers,
		const unsigned int* pStrides,
		const unsigned int* pOffsets) noexcept;

/**
 * @brief Entrega a la tarjeta la caja que dice en qué orden unir
 * los puntos.
 * @param pIndexBuffer La caja con ese orden.
 * @param format: Tamaño de cada número de la lista
 * @param offset: Cuántos bytes saltar al inicio de la caja antes
 * de empezar a leer.
 */
void
	IASetIndexBuffer(ID3D11Buffer* pIndexBuffer,
		DXGI_FORMAT format,
		unsigned int offset) noexcept;

void
	VSSetShader(ID3D11VertexShader* pVertexShader,
		ID3D11ClassInstance* const* ppClassInstances = nullptr,
		unsigned int numClassInstances = 0) noexcept;

/**
 * @brief Elige el programa que decide de qué color se pinta cada
 * pedacito de la figura
 * @param pPixelShader: El programa que se quiere usar
 * @param ppClassInstances: Opción avanzada; normalmente va vacía
 * @param numClassInstances: Cantidad de opciones avanzadas
 */
void
	PSSetShader(ID3D11PixelShader* pPixelShader,
		ID3D11ClassInstance* const* ppClassInstances = nullptr,
		unsigned int numClassInstances = 0) noexcept;

void
	VSSetConstantBuffers(unsigned int startSlot,
		unsigned int numBuffers,
		ID3D11Buffer* const* ppConstantBuffers) noexcept;

/**
 * @brief Cambia el contenido de una caja o imagen de la tarjeta de
 * video por datos nuevos
 * @param pDstResource: La caja o imagen que se quiere actualizar.
 * @param dstSubresource Qué parte se actualiza
 * @param pSrcData: Los datos nuevos que se copian.
 * @param srcDepthPitch: Tamaño de cada capa de datos 
 */
void
	UpdateSubresource(ID3D11Resource* pDstResource,
		unsigned int dstSubresource,
		const D3D11_BOX* pDstBox,
		const void* pSrcData,
		unsigned int srcRowPitch,
		unsigned int srcDepthPitch) noexcept;

/**
	 * @brief Dibuja la figura usando la lista que indica en que orden
	 *unir los puntos
	 * Es la orden final: todo lo anterior prepara la escena y esta
	 * función hace que realmente aparezca en pantalla
	 */
void
	DrawIndexed(unsigned int indexCount,
		unsigned int startIndexLocation = 0,
		int baseVertexLocation = 0) noexcept;


private:
/**
* @brief El director real de la tarjeta de video
* Empieza vacío nulo y se llena al llamar a init()
*/
ID3D11DeviceContext* m_deviceContext = nullptr;
};