#pragma once
#include "Engine/Prerequisites.h"


class Window;
class Device;
class Texture2D;

/**
* @class SwapChain
* @brief Es el encargado de mostrar en pantalla lo que la tarjeta de video dibuja.
* 
* Para que la imagen no se vea cortada o parpadeando, la tarjeta de video
* no dibuja directamente sobre lo que ves. Dibuja en una imagen oculta
* y, cuando termina, la intercambia con la que esta en pantalla
* Esta clase se encarga de manejar ese intercambio
*/
class
SwapChain {
public:
SwapChain() = default;
~SwapChain();

/**
* @brief No se permite hacer copias de este objeto
* Solo puede existir un intercambiador por ventana
*/
SwapChain(const SwapChain&) = delete;
SwapChain& operator=(const SwapChain&) = delete;

/**
 * @brief No se permite mover este objeto a otro lugar.
 */
SwapChain(SwapChain&&) = delete;
SwapChain& operator=(SwapChain&&) = delete;

/**
 * @brief Prepara el intercambiador para mostrar imágenes en una ventana
 * @param device Fabricante de la tarjeta de video
 * @param window La ventana donde se mostrarán las imágenes
 * @param width  Ancho de la imagen, en píxeles
 * @param height Alto de la imagen, en píxeles
 * @return Un resultado que dice si todo salió bien o si hubo un error
 */
HRESULT
	init(Device device, Window window,
		unsigned int width, unsigned int height);

void
	destroy();

/**
 * @brief Muestra en pantalla el cuadro que ya terminó de dibujarse
 * Es el momento en que la imagen oculta pasa a ser la visible
 * @param syncInterval Cuántas veces espera la pantalla antes de
 * mostrar el cuadro Con 1 se sincroniza con la
 * pantalla mas suave, con 0 se muestra de
 * inmediato
 */
HRESULT
	Present(unsigned int syncInterval = 1,
		unsigned int flags = 0);

/**
 * @brief Obtiene la imagen oculta sobre la que se va a dibujar
 * @param bufferIndex Número de la imagen que se quiere
 * @param backBuffer  Aquí se guarda la imagen obtenida
 * @return Un resultado que dice si todo salió bien o si hubo un error
 */
HRESULT
	GetBuffer(unsigned int bufferIndex,
		Texture2D backBuffer);

/**
 * @brief Ajusta el tamaño de las imágenes cuando cambia el tamaño
 * de la ventana
 * @param width  Nuevo ancho, en píxeles
 * @param height Nuevo alto, en píxeles
 * @return Un resultado que dice si todo salió bien o si hubo un error 
 */
HRESULT
	ResizeBuffers(unsigned int width,
		unsigned int height);

/**
 * @brief Entrega el intercambiador real de DirectX
 * @return El intercambiador 
 */
IDXGISwapChain*
	getSwapChain() { return m_swapChain; }

private:
/**
* @brief El intercambiador real de DirectX
* Empieza vacío (nulo) y se llena al llamar a init()
*/
IDXGISwapChain* m_swapChain = nullptr;
};