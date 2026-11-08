#pragma once
#include "Engine/Prerequisites.h"
#include "Engine/MeshComponent.h"

class Device;
class DeviceContext;

/**
 * @class Buffer
 * @brief Una "caja" en la memoria de la tarjeta de video que guarda datos
 * La tarjeta de video no puede leer directamente la memoria normal de
 * la computadora. Para que pueda usar los datos de una figura 
 * primero hay que copiarlos a una caja
 * especial dentro de la tarjeta. Esta clase se encarga de crear esa caja,
 * llenarla, actualizarla, usarla para dibujar y borrarla al terminar.
 */
class
    Buffer {
public:

    Buffer() = default;

    ~Buffer() = default;

    /**
   * @brief Crea la caja y la llena con los datos de una figura 3D
   * Toma los puntos o las conexiones de la malla y los copia a la
   * tarjeta de video, listos para dibujar
   * @param device   La tarjeta de video con la que se va a trabajar
   * @param mesh     La figura 3D de donde se sacan los datos
   * @param bindFlag Indica para qué se usará la caja
   * @return Un resultado que dice si todo salió bien o si hubo un error
   */
    HRESULT
        init(Device& device, const MeshComponent& mesh, unsigned int bindFlag);

    /**
    * @brief Crea una caja vacía de un tamaño determinado
    * Sirve para datos que cambian seguido
    * y que se llenarán después con update()
    * @param device    La tarjeta de video con la que se va a trabajar
    * @param ByteWidth El tamaño de la caja, medido en bytes
    * @return Un resultado que dice si todo salió bien o si hubo un error
    */
    HRESULT
        init(Device& device, unsigned int ByteWidth);

    /**
 * @brief Le dice a la tarjeta de video que use esta caja para dibujar.
 *
 * @param deviceContext  El "mensajero" que envía las órdenes a la
 *  tarjeta de video
 * @param StartSlot El numero de lugar donde se va a colocar la caja
 * @param NumBuffers Cuantas cajas se van a colocar
 * @param setPixelShader Si es verdadero, la caja también se pone a
 *  disposición del programa que pinta los colores
 * @param format Forma en que están escritos los dato
 */
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

    /**
   * @brief Fabrica la caja en la tarjeta de video a partir de una descripción
   * Es el paso de bajo nivel que usan los dos init() para crear la caja
   * @param device   La tarjeta de video con la que se va a trabajar
   * @param initData Los datos con los que se llena al inicio
   * @return Un resultado que dice si todo salió bien o si hubo un error
   */
    HRESULT
        createBuffer(Device& device,
            D3D11_BUFFER_DESC& desc,
            D3D11_SUBRESOURCE_DATA* initData);

    /**
    * @brief Devuelve la caja de la tarjeta de video para usarla directamente
    * @return Un puntero a la caja
    */
    ID3D11Buffer* getBuffer() const { return m_buffer; }

public:
    ID3D11Buffer* m_buffer = nullptr;
private:
    unsigned int m_stride = 0;
    unsigned int m_offset = 0;
    unsigned int m_bindFlag = 0;
};