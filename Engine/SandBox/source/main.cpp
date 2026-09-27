#include <Engine/Engine.h>
#include <Window.h>


/**
 * @brief Punto de entrada principal de la aplicacion en Windows
 * se encarga de crear la ventana, inicializar el engin
 * ejecutar el ciclo principal de la aplicación y liberar
 * los recursos cuando la ventana se cierra
 * previousInstance Instancia anterior de la aplicacion
 * @param instance Instancia actual de la aplicacion
 * @param commandLine Argumentos recibidos desde la línea de comandos.
 * @param showCommand Indica cómo se debe mostrar la ventan
 * @return 0 si la aplicación termina correctamente
 * @return 1 si ocurre un error al crear la ventana o inicializar el engine
 */

int WINAPI
wWinMain(HINSTANCE previousInstance,
    HINSTANCE instance,
    PWSTR commandLine,
    int showCommand){

    // Eestos parmetros no se utilizan en esta aplicacion
    UNREFERENCED_PARAMETER(previousInstance);
    UNREFERENCED_PARAMETER(commandLine);

    //tamaño de la ventana 
    constexpr UINT CLIENT_WIDTH = 1280;
    constexpr UINT CLIENT_HEIGHT = 720;

    Window window;

  
    //Aqui se crea la ventana y configura el tamaño y titulo
    if (!window.Create(instance, L"Spider Engine", CLIENT_WIDTH, CLIENT_HEIGHT))
    {
        MessageBoxW(
            nullptr,
            L"No se pudo crar la ventana. ",
            L"Sandbox Error",
            MB_OK | MB_ICONERROR
        );
        return 1; 
    }
   
    
    Engine engine;
    //Inicialza el engine utilizando la ventana creada
    if (!engine.Initialize(window.GetHandle(), CLIENT_WIDTH, CLIENT_HEIGHT))
    {
        MessageBoxW(
            window.GetHandle(),
            L"No se pudo inicializar el Engine. \n\n"
            L"Verifica que exista:\n"
            L"shaders\\Cube.hlsl\n\n"
            L"Reisa tambien la ventana Output. ",
            L"Engine Error",
            MB_OK | MB_ICONERROR
        );

            return 1;
    }

    //muestra la ventana en la pantalla
    window.Show(showCommand);


    //ciclo principal de la aplicacion
    while (window.ProcessMessages())
    {
        //si se minimiza la ventana no es necesario renderizarla
        if (window.IsMinimized())
        {
            WaitMessage();
            continue;
        }
        engine.Render();
    }
    
    //libera los recrusos utilizados por el engine
    engine.Shutdown();

    //destruye la ventana y libera sus recursos
    window.Destroy();

    return 0;

   }