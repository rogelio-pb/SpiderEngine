#pragma once
#include "Engine/Prerequisites.h"

class Device;
class DeviceContext;

/**
* @class Texture2D
* @brief Texture 2D funciona como una imagen que vive en la tarjeta de video.
* 
* Sirve para varias cosas: puede ser una foto o dibujo que cubre un
* objeto (una "textura"), la imagen donde se va a pintar la escena, o
* la imagen que guarda qué tan lejos está cada cosa. Esta clase crea
* esa imagen, la entrega a la tarjeta y la borra al terminar
*/
class
Texture2D {
public:
Texture2D() noexcept = default;
~Texture2D() noexcept;

Texture2D(const Texture2D&) = delete;
Texture2D& operator=(const Texture2D&) = delete;

Texture2D(Texture2D&&) = delete;
Texture2D& operator=(Texture2D&&) = delete;

/**
* @brief Crea la imagen cargándola desde un archivo
* @param device: Fabricante de la tarjeta de video
* @param textureName: Nombre del archivo de imagen
* @return Un resultado que dice si todo salió bien o si hubo un error
*/
HRESULT
    init(Device& device,
        const std::string& textureName,
        ExtensionType extensionType);

/**
   * @brief Crea una imagen vacia de un tamaño determinado
   * Sirve, por ejemplo, para crear la imagen donde se va a dibujar
   * o la imagen que guarda las distancias
   * @param device: Fabricante de la tarjeta de video
   * @param width: Ancho de la imagen, en píxeles
   * @param height: Alto de la imagen, en píxeles
   * @param Format: Cómo se guarda el color de cada pixel
   * @param BindFlags: Para qué se usará la imagen
   * @param sampleCount: Cuántas muestras se toman por pixel para
   * suavizar los bordes
   * @param: qualityLevels Nivel de calidad de ese suavizado
   * @return: Un resultado que dice si todo salió bien o si hubo un error
   */
HRESULT
    init(Device& device,
        unsigned int width,
        unsigned int height,
        DXGI_FORMAT Format,
        unsigned int BindFlags,
        unsigned int sampleCount = 1,
        unsigned int qualityLevels = 0);

/**
 * @brief Crea una imagen nueva con las mismas características de
 * otra imagen, pero con otro formato de color
 * @param device: Fabricante de la tarjeta de video
 * @param textureRef La imagen que se toma como modelo
 * @param format: como se guarda el color de cada píxel en la imagen nueva
 * @return Un resultado que dice si todo salió bien o si hubo un error
 */
HRESULT
    init(Device& device, Texture2D& textureRef, DXGI_FORMAT format);

/**
 * @brief Carga una imagen desde una ruta completa del disco
 * @param device   Fabricante de la tarjeta de video
 * @param fullPath Ubicación completa del archivo, con todo y carpetas
 * @param texture  La imagen donde se guarda lo cargado
 * @return Un resultado que dice si todo salió bien o si hubo un error
 */
HRESULT
    InitTextureFromImage(Device& device,
        const std::string& fullPath,
        Texture2D& texture);

//metodo para renderizar la textura
void
    render(DeviceContext& deviceContext, unsigned int StartSlot, unsigned int NumViews);


void
    destroy();

/**
  * @brief Crea un cubo de imágenes a partir de seis fotos
  * Un cubemap es una imagen formada por seis caras, como un dado
  * Se usa mucho para crear cielos o fondos que rodean toda la escen
  * @param device: Fabricante de la tarjeta de video
  * @param deviceContext: Envía las órdenes a la tarjeta de video
  * @param facePaths: Las seis rutas de las imágenes, una por cada cara del cubo
  * @param generateMips: Si es verdadero, se crean versiones más
  * pequeñas de la imagen para que se vea mejor de lejos
  * @return Un resultado que dice si todo salió bien o si hubo un error
  */
HRESULT
    CreateCubemap(Device& device,
        DeviceContext& deviceContext,
        const std::array<std::string, 6>& facePaths,
        bool generateMips /*= false*/);

/**
  * @brief Crea una "ventana de lectura" para ver una sola cara del cubo de imágenes
  * Sirve para que un programa de dibujo pueda leer solo una de las
  * seis caras, sin necesidad de usar todo el cubo
  * @param device: Fabricante de la tarjeta de video
  * @param cubemapTex El cubo de imágenes completo
  * @param format: Cómo se guarda el color de cada píxel
  * @param faceIndex: Número de la cara que se quiere ver 
  * @param mipLevels: Cuántas versiones reducidas de la imagen se
  * pueden usar
  * @return La ventana de lectura creada, o nulo si algo falló
  * @note Quien la reciba debe liberarla cuando ya no la necesite
  */
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

/**
* @brief Entrega la imagen real guardada en la tarjeta de video.
*/
ID3D11Texture2D*
    GetTexture() const noexcept {
    return m_texture;
}

/**
* @brief Entrega la direccion donde vive la imagen
* Se usa cuando otra función necesita guardar ahí una imagen
* recién creada, por ejemplo, al obtener la imagen de la pantalla
* @return La dirección donde se guarda la imagen
*/
ID3D11Texture2D**
    GetTextureAddress() noexcept {
    return &m_texture;
}


public:
ID3D11Texture2D* m_texture = nullptr;
ID3D11ShaderResourceView* m_textureFromImg = nullptr;
std::string m_textureName;
};

