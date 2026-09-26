#include <Engine/Engine.h>


#include <DirectXMath.h>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <sstream>
#include <cstddef>
#include <chrono>
#include <cstdint>
#include <new>

// MACROS
/**
 * @brief Libera un recurso de DirectX y pone el puntero en nullptr
 * Primero comprueba que el recurso exista, lo libera y después
 * deja el puntero vacío para evitar usarlo por accidente
 */
#define SAFE_RELEASE(x) if(x != nullptr) x->Release(); x = nullptr;

 /**
  * @brief Muestra un mensaje en la consola de depuración
  * Se utiliza principalmente para saber cuando se crea un recurso
  * y poder revisar que est pasando dentro del motor
  * @param classObj Nombre de la clase
  * @param method Nombre del meetodo
  * @param state Mensaje que indica el estado de la creacion
  */
#define MESSAGE( classObj, method, state )   \
{                                            \
   std::wostringstream os_;                  \
   os_ << classObj << "::" << method << " : " << "[CREATION OF RESOURCE " << ": " << state << "] \n"; \
   OutputDebugStringW( os_.str().c_str() );  \
}

  /**
   * @brief Muestra un mensaje de error en la consola de depuracion
   * Se utiliza para saber en qué clase y mtodo ocurrio un error
   * y mostrar un mensaje que ayude a encontrar el problema

   */
#define ERROR(classObj, method, errorMSG)                     \
{                                                             \
    try {                                                     \
        std::wostringstream os_;                              \
        os_ << L"ERROR : " << classObj << L"::" << method     \
            << L" : " << errorMSG << L"\n";                   \
        OutputDebugStringW(os_.str().c_str());                \
    } catch (...) {                                           \
        OutputDebugStringW(L"Failed to log error message.\n");\
    }                                                         \
}

   /**
    * @brief Libera de forma segura un recurso
    * Si el objeto existe, llama a Release() para liberar el recurso
    * y después pone el puntero en nullptr
    * @tparam T Tipo del recurso que queremos liberar
    * @param object Recurso que se quiere liberar
    */

template<typename T>
void SafeRelease(T * &object) noexcept
{
    if (object != nullptr)
    {
        object->Release();
        object = nullptr;
    }
}

/**
 * @brief Guarda todos los datos internos que necesita el motor
 * implementation contiene las cosas que Engine necesita para
 * trabajar internamente, pero que no necesitamos mostrar
 * directamente en Engine.h
 */
struct 

//Representa un vertise que se puede dibujar, guarda el color y su posicion
Engine::Implementation 
{
    struct Vertex
    {
        float position[3];
        float color[4];
    };

    /**
     * @brief Guarda la información necesaria para transformar
     * los objetos que se van a dibujar.
     * El alignas se utiliza porque DirectX necesita que
     * esta información esté acomodada correctamente en memoria.
     */
    struct alignas(16) TransformBuffer
    {
        DirectX::XMFLOAT4X4 worldViewProjection;
    };

    //Ventana donde se mostrara la figura
    HWND window = nullptr;

    //Es el ancho de la ventana
    std::uint32_t width = 0;

    //Es el alto de la ventana
    std::int32_t height = 0;

    //SSe encarga de crear y administrar recursos de DirectX
    ID3D11Device* device = nullptr;

    //Se encarga de ejecutar las instrucciones de dibujo
    ID3D11DeviceContext* context = nullptr;

    //Se encarga de mostrar en pantalla los cuadros que genera el motor
    IDXGISwapChain* swapChain = nullptr;

    // Representa la imagen donde se dibuja el resultado final
    ID3D11RenderTargetView* renderTarget = nullptr;

    //Guarda la información de profundidad de los objetos
    ID3D11Texture2D* depthStencilBuffer = nullptr;

    //Permite utilizar la información de profundidad  al momento de dibujar
    ID3D11DepthStencilView* depthStencilView = nullptr;

    //Buffers***

    //Guarda los vertices que se van a dibujar
    ID3D11Buffer* vertexBuffer = nullptr;
    //Guarda el orden en que se utilizan los vrtices
    ID3D11Buffer* indexBuffer = nullptr;
    //Guarda información para transformar los objetos antes de dibujarlos
    ID3D11Buffer* transformBuffer = nullptr;

    //Guarda la configuración utilizada para decidir como se dibujaran las caras
    ID3D11RasterizerState* rasterizerState = nullptr;

    //Guarda cuando inicio el motor
    std::chrono::steady_clock::time_point startTime{};

    //Shader que se encarga de procesar los vertices
    ID3D11VertexShader* vertexShader = nullptr;
    //Shader que se encarga de definir el resultado de como se ven los pixeles
    ID3D11PixelShader* pixelShader = nullptr;
    //Indica a DirectX cómo están organizados los datos de los vertices
    ID3D11InputLayout* inputLayout = nullptr;
  

    /**
    * @brief Compila un shader desde un archivo
    * Busca el shader en el archivo indicado y lo convierte
    * a una forma que DirectX pueda utilizar
    * @param filename Archivo donde se encuentra el shader
    * @param entryPoint Punto donde comienza el shader
    * @param shaderModel Versión del shader que se utilizara
    * @param shaderBlod Lugar donde se guardará el shader compilado
    * @return true si el shader se compiló correctamente,
    *         false si ocurrió algún error
    */
    static bool 
        CompileShader(const wchar_t* filename, const char* entryPoint,
            const char* shaderModel, ID3DBlob** shaderBlod) noexcept {
        if (!filename || !entryPoint || !shaderModel || !shaderBlod) {
            return false;
        }

        *shaderBlod = nullptr;

        UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS; 

/**
* @brief Configuraciones utilizadas al compilar el shader.
* En Debug se agregan opciones para poder encontrar errores
* más fácilment en Release se activa la optimizacioon
*/
#ifdef _DEBUG
        compileFlags |= D3DCOMPILE_DEBUG;
        compileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

        ID3DBlob* errors = nullptr;

        /**
        * @brief Intenta compilar el shader
        */
        const HRESULT result = D3DCompileFromFile(
            filename,
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint,
            shaderModel,
            compileFlags,
            0,
            shaderBlod,
            &errors
        );

        /**
        * @brief Si hubo errores o mensajes durante la compilación,
        * los mostramos en la ventana de depuración.
        */
        if (errors)
        {
            OutputDebugStringA(
                static_cast<const char*>(
                    errors->GetBufferPointer()
                    )
            );
            SafeRelease(errors);
        }
        
        /**
        * @brief Si la compilación falló, liberamos el shader
        * y avisamos que no se pudo crear.
        */
        if (FAILED(result))
        {
            SafeRelease(*shaderBlod);
            return false;
        }
        return true;
    }

    /**
    * @brief Libera todos los recursos utilizados por el motor
    * Se llama cuando vamos a cerrar el motor para asegurarnos
    * de que DirectX libere correctamente todo lo que utilizamos
    */
    void ReleaseResources() noexcept
    {
        if (context)
        {
            context->ClearState();
            context->Flush();
    }
        //Se liberan los recursos
        SafeRelease(rasterizerState);
        SafeRelease(transformBuffer);
        SafeRelease(indexBuffer);
        SafeRelease(vertexBuffer);

        SafeRelease(inputLayout);
        SafeRelease(pixelShader);
        SafeRelease(vertexShader);

        SafeRelease(depthStencilView);
        SafeRelease(depthStencilBuffer);
        SafeRelease(renderTarget);

        SafeRelease(swapChain);
        SafeRelease(context);
        SafeRelease(device);

        //Regresa todo a su estado inicial
        window = nullptr;
        width = 0;
        height = 0;
    }
};

/**
 * @brief Crea el motor y prepara su implementación interna
 * new (std::nothrow) intenta crear la memoria sin lanzar una
 * excepcion si no hay suficiente memoria
 */
Engine::Engine() noexcept
    :m_implementation(
        new (std::nothrow) Implementation{}
    )
{
}

//Destructor del motor que libera los recursos
Engine::~Engine() noexcept
{
    Shutdown();

    delete m_implementation;
    m_implementation = nullptr;
}

/**
 * @brief inicia el motor y prepara todo lo necesario para
 * comenzar a utilizarlo
 * @param nativeWindow Ventana donde se mostrar el motor
 * @param width Ancho de la ventana
 * @param height Alto de la ventana
 * @return truee si la inicializacionmn puede continuar,
 *false si falta algún dato necesario
 */
bool Engine::Initialize(
    void* nativeWindow,
    std::uint32_t width,
    std::uint32_t height

) noexcept
{
    /**
   * @brief Comprobamos que tengamos la implementacion,
   * la ventana y dimensiones validas
   * si algo falta, no podemos iniciar correctamente el motor
   */
    if (!m_implementation ||
        !nativeWindow ||
        width == 0 ||
        height == 0)
    {
        return false;
    }
	Implementation& engine = *m_implementation;

    engine.ReleaseResources();

    engine.window = static_cast<HWND>(nativeWindow);
    engine.width = width;
    engine.height = height;
    DXGI_SWAP_CHAIN_DESC swapChainDescription{};

    swapChainDescription.BufferCount = 2;
    swapChainDescription.BufferDesc.Width = engine.width;
    swapChainDescription.BufferDesc.Height = engine.height;
    swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDescription.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDescription.OutputWindow = engine.window;
    swapChainDescription.SampleDesc.Count = 1;
    swapChainDescription.SampleDesc.Quality = 0;
	swapChainDescription.Windowed = TRUE;
    swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    constexpr D3D_FEATURE_LEVEL featureLevels[]{
        D3D_FEATURE_LEVEL_11_0
    };
    
	D3D_FEATURE_LEVEL selectedFeatureLevel{};

    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &swapChainDescription,
        &engine.swapChain,
        &engine.device,
        &selectedFeatureLevel,
        &engine.context
    );

    if (FAILED(result))
    {
        SafeRelease(engine.swapChain);
        SafeRelease(engine.context);
        SafeRelease(engine.device);

        result = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_WARP,
            nullptr,
            0,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            &swapChainDescription,
            &engine.swapChain,
            &engine.device,
            &selectedFeatureLevel,
            &engine.context
        );
    }
    if (FAILED(result))
    {
        engine.ReleaseResources();
        return false;
    }
	ID3D11Texture2D* backBuffer = nullptr;
    result = engine.swapChain->GetBuffer(
        0,
        __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&backBuffer)
    );

    if(FAILED(result))
    {
        ERROR("Engine", "Initialize", ("Failed to get swap chain buffer"
              + std::to_string(result)).c_str());
        engine.ReleaseResources();
        return false;
    }
    result = engine.device->CreateRenderTargetView(
        backBuffer,
        nullptr,
        &engine.renderTarget
    );
    SafeRelease(backBuffer);

    if (FAILED(result))
    {
        engine.ReleaseResources();
        return false;
    }

    D3D11_TEXTURE2D_DESC depthBufferDescription{};  
    depthBufferDescription.Width = engine.width;
    depthBufferDescription.Height = engine.height;
    depthBufferDescription.MipLevels = 1;
    depthBufferDescription.ArraySize = 1;
    depthBufferDescription.Format =
        DXGI_FORMAT_D24_UNORM_S8_UINT;

    depthBufferDescription.SampleDesc.Count = 1;
    depthBufferDescription.SampleDesc.Quality = 0;
    depthBufferDescription.Usage = D3D11_USAGE_DEFAULT;

    depthBufferDescription.BindFlags =
        D3D11_BIND_DEPTH_STENCIL;

    result = engine.device->CreateTexture2D(
        &depthBufferDescription,
        nullptr,
        &engine.depthStencilBuffer
    );

    if (FAILED(result))
    {
        engine.ReleaseResources();
        return false;
    }

    result = engine.device->CreateDepthStencilView(
        engine.depthStencilBuffer,
        nullptr,
        &engine.depthStencilView
        );

    if (FAILED(result))
    {
        engine.ReleaseResources();
        return false;
    }


    D3D11_VIEWPORT viewport{};

    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    viewport.Width =
        static_cast<float>(engine.width);

    viewport.Height =
        static_cast<float>(engine.height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    engine.context->RSSetViewports(
        1,
        &viewport
    );

    ID3DBlob* vertexShaderBlob = nullptr;
    ID3DBlob* pixelShaderBlob = nullptr;

    if (!Implementation::CompileShader(
        L"shaders\\Cube.hlsl",
        "VSMain",
        "vs_5_0",
        &vertexShaderBlob))
    {
        engine.ReleaseResources();
        return false;
    }

    if (!Implementation::CompileShader(
        L"shaders\\Cube.hlsl",
        "PSMain",
        "ps_5_0",
        &pixelShaderBlob))
    {
        SafeRelease(vertexShaderBlob);
        engine.ReleaseResources();
        return false;
    }
    result = engine.device->CreateVertexShader(
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        nullptr,
        &engine.vertexShader
    );

    if (FAILED(result))
    {
        SafeRelease(pixelShaderBlob);
        SafeRelease(vertexShaderBlob);
		engine.ReleaseResources();
        return false;
    }
    result = engine.device->CreatePixelShader(
        pixelShaderBlob->GetBufferPointer(),
        pixelShaderBlob->GetBufferSize(),
        nullptr,
        &engine.pixelShader
    );
    
    if (FAILED(result))
    {
		SafeRelease(pixelShaderBlob);
		SafeRelease(vertexShaderBlob);
        engine.ReleaseResources();
        return false;
    }

    constexpr D3D11_INPUT_ELEMENT_DESC inputElements[]{
        {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, static_cast<UINT>
        (offsetof(Implementation::Vertex, position)), D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"Color", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, static_cast<UINT>
        (offsetof(Implementation::Vertex, color)), D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    result = engine.device->CreateInputLayout(
        inputElements,
        ARRAYSIZE(inputElements),
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        &engine.inputLayout
    );

    SafeRelease(pixelShaderBlob);
    SafeRelease(vertexShaderBlob);

	if (FAILED(result))
	{
		engine.ReleaseResources();
		return false;
	}

    //******************
    constexpr Implementation::Vertex vertices[]
    {
        // Frente
        {
            { -1.0f,  1.0f, -1.0f },
            {  1.0f,  0.0f,  0.0f, 1.0f }
        },
        {
            {  1.0f,  1.0f, -1.0f },
            {  0.0f,  1.0f,  0.0f, 1.0f }
        },
        {
            {  1.0f, -1.0f, -1.0f },
            {  0.0f,  0.0f,  1.0f, 1.0f }
        },
        {
            { -1.0f, -1.0f, -1.0f },
            {  1.0f,  1.0f,  0.0f, 1.0f }
        },

        // Atrás
        {
            { -1.0f,  1.0f, 1.0f },
            {  1.0f,  0.0f, 1.0f, 1.0f }
        },
        {
            {  1.0f,  1.0f, 1.0f },
            {  0.0f,  1.0f, 1.0f, 1.0f }
        },
        {
            {  1.0f, -1.0f, 1.0f },
            {  1.0f,  1.0f, 1.0f, 1.0f }
        },
        {
            { -1.0f, -1.0f, 1.0f },
            {  0.2f,  0.4f, 1.0f, 1.0f }
        }
    };

    D3D11_BUFFER_DESC vertexBufferDescription{};
    
    vertexBufferDescription.ByteWidth =
        static_cast<UINT>(sizeof(vertices));

    vertexBufferDescription.Usage =
        D3D11_USAGE_IMMUTABLE;

    vertexBufferDescription.BindFlags =
        D3D11_BIND_VERTEX_BUFFER;

    vertexBufferDescription.CPUAccessFlags = 0;
    vertexBufferDescription.MiscFlags = 0;
    vertexBufferDescription.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA initialVertexData{};
    initialVertexData.pSysMem = vertices;

    result = engine.device->CreateBuffer(
        &vertexBufferDescription,
        &initialVertexData,
        &engine.vertexBuffer
    );

    if (FAILED(result))
    {
        engine.ReleaseResources();
        return false;
    }

    constexpr std::uint16_t indices[]
    {
        // Frente
        0, 1, 2,
        0, 2, 3,

        // Atrás
        5, 4, 7,
        5, 7, 6,

        // Izquierda
        4, 0, 3,
        4, 3, 7,

        // Derecha
        1, 5, 6,
        1, 6, 2,

        // Arriba
        4, 5, 1,
        4, 1, 0,

        // Abajo
        3, 2, 6,
        3, 6, 7
    };

    D3D11_BUFFER_DESC indexBufferDescription{};
    
    indexBufferDescription.ByteWidth = static_cast<UINT>(sizeof(indices));
    indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexData{};
    indexData.pSysMem = indices;

    result = engine.device->CreateBuffer
    (&indexBufferDescription,&indexData,&engine.indexBuffer);

    if (FAILED(result))
    {
        engine.ReleaseResources();
        return false;
    }

    D3D11_BUFFER_DESC transformBufferDescription{};

    transformBufferDescription.ByteWidth =
        sizeof(Implementation::TransformBuffer);

    transformBufferDescription.Usage = D3D11_USAGE_DEFAULT;
    transformBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    result = engine.device->CreateBuffer
    (&transformBufferDescription, nullptr, &engine.transformBuffer);

    if (FAILED(result))
    {
        engine.ReleaseResources();
        return false;
    }

    D3D11_RASTERIZER_DESC rasterizerDescription{};
    rasterizerDescription.FillMode = D3D11_FILL_SOLID;
    rasterizerDescription.CullMode = D3D11_CULL_NONE;
    rasterizerDescription.DepthClipEnable = TRUE;

    result = engine.device->CreateRasterizerState
    (&rasterizerDescription, &engine.rasterizerState);

    if (FAILED(result))
    {
        engine.ReleaseResources();
        return false;
    }

    engine.startTime = std::chrono::steady_clock::now();

    return true;
}

void Engine::Render() noexcept
{
    if (!m_implementation)
        return;

    Implementation& engine = *m_implementation;

    if (!engine.context ||
        !engine.swapChain ||
        !engine.renderTarget ||
        !engine.vertexBuffer ||
        !engine.indexBuffer ||
        !engine.transformBuffer ||
        !engine.inputLayout ||
        !engine.vertexShader ||
        !engine.pixelShader)
    {
        return;
    }

    constexpr float clearColor[]
    {
        0.03f,
        0.04f,
        0.08f,
        1.0f
    };

    engine.context->OMSetRenderTargets(
        1,
        &engine.renderTarget,
        engine.depthStencilView
    );

    engine.context->ClearRenderTargetView(
        engine.renderTarget,
        clearColor
    );

    engine.context->ClearDepthStencilView(
        engine.depthStencilView,
        D3D11_CLEAR_DEPTH |
        D3D11_CLEAR_STENCIL,
        1.0f,
        0
    );


    const auto currentTime =
        std::chrono::steady_clock::now();

    const float elapsedSeconds =
        std::chrono::duration<float>(
            currentTime - engine.startTime
        ).count();

    using namespace DirectX;

    const XMMATRIX world =
     XMMatrixRotationX(elapsedSeconds * 0.4f) * XMMatrixRotationY(elapsedSeconds * 0.8f);

    const XMVECTOR cameraPosition = XMVectorSet(0.0f, 2.0f, -5.0f, 1.0f);

    const XMVECTOR cameraTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

    const XMVECTOR cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    const XMMATRIX view = XMMatrixLookAtLH(cameraPosition, cameraTarget, cameraUp);

    const float aspectRatio =
        static_cast<float>(engine.width) /
        static_cast<float>(engine.height);

    const XMMATRIX projection =
        XMMatrixPerspectiveFovLH(
            XM_PIDIV4,
            aspectRatio,
            0.1f,
            100.0f
        );

    Implementation::TransformBuffer transform{};

    XMStoreFloat4x4(
        &transform.worldViewProjection,
        XMMatrixTranspose(
            world * view * projection
        )
    );

    engine.context->UpdateSubresource(
        engine.transformBuffer, 0, nullptr, &transform, 0, 0
    );

    constexpr UINT stride =
        sizeof(Implementation::Vertex);

    constexpr UINT offset = 0;

    engine.context->IASetVertexBuffers(
        0,
        1,
        &engine.vertexBuffer,
        &stride,
        &offset
    );

    engine.context->IASetIndexBuffer(
        engine.indexBuffer, DXGI_FORMAT_R16_UINT, 0
    );

    engine.context->IASetInputLayout(
        engine.inputLayout
    );

    engine.context->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    engine.context->RSSetState(
        engine.rasterizerState
    );

    engine.context->VSSetShader(
        engine.vertexShader,
        nullptr,
        0
    );

    engine.context->VSSetConstantBuffers(
        0,
        1,
        &engine.transformBuffer
    );

    engine.context->PSSetShader(
        engine.pixelShader,
        nullptr,
        0
    );

    engine.context->PSSetShader(
        engine.pixelShader,
        nullptr,
        0
    );

    engine.context->PSSetShader(
        engine.pixelShader,
        nullptr,
        0
    );

    engine.context->DrawIndexed(36, 0, 0);

    engine.swapChain->Present(1, 0);
}

void Engine::Shutdown() noexcept
{
    if (m_implementation)
        m_implementation->ReleaseResources();
}



