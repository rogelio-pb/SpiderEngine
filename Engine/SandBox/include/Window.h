#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>


/**
 * @brief Representa y administra una ventana de Windows
 * la clase se encarga de crear mostrar procesar mensajes
 * y destruir la ventana que utiliza el Engine
 */
class Window final {
public:
	Window() = default;
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;


	/**
	* @brief Crea la ventana de Windows
	* Registra la clase de ventana y crea la ventana
	* con el tamaño indicado
	* @param instance Instancia de la aplicacion
	* @param tittle Título que tendrá la ventana
	* @param clientWidth Ancho del area interna
	* @param clientHeight Alto del area interna
	* @return true si la ventana se creo correctamente
	* @return false si ocurrió algun error
	*/
	bool
		Create(HINSTANCE instance, const wchar_t* tittle,
			UINT clientWidth, UINT clientHeight) noexcept;

	//Indica como debe mostrar la ventana
	void
		Show(int showCommand) noexcept;

	//Destruye la ventana y libera sus recursos
	void
		Destroy() noexcept;

	//Devuelve false cuando se recibe WM_QUIT.
	bool
		ProcessMessages() noexcept;

	HWND
		GetHandle() const noexcept;

	//es verdadero si la ventana es minimizada
	bool
		IsMinimized() const noexcept;

private:
	/**
	 * @brief Procesa los mensajes recibidos por la ventana
	 * Windows utiliza esta función para enviar eventos
	 * a la ventana, como cerrarla o actualizarla
	 * @param handle Identificador de la ventana
	 * @param message Tipo de mensaje recibido
	 * @param aParam Información adicional del mensaje
	 * @param lParam Información adicional del mensaje
	 * @return Resultado del procesamiento del mensaje
	 */
	static LRESULT CALLBACK
		WindowProcedure(HWND handle, UINT message,
			WPARAM aParam, LPARAM lParam);

	static constexpr const wchar_t* ClassName =
		L"SpiderEngineWindow";

	HINSTANCE m_instance = nullptr;
	HWND m_handle = nullptr;
	bool m_classRegistered = false;
};
