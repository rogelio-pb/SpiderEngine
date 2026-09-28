#include "Window.h"


Window::~Window()
{
	Destroy();
}

bool
Window::Create(HINSTANCE instance, const wchar_t* tittle,
	UINT clientWidth, UINT clientHeight) noexcept {
	if (m_handle || !instance || !tittle ||
		clientWidth == 0 || clientHeight == 0)
	{
		return false;
	}

	//configuracion de la clase de ventana
	WNDCLASSEXW windowClass{};
	windowClass.cbSize = sizeof(windowClass);

	//funcion que recibira los mensajes de Windows
	windowClass.lpfnWndProc = WindowProcedure;
	//instancia de la aplicacion
	windowClass.hInstance = instance;
	//cursor que utilizara la ventaana
	windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	//nombre de la clase de la ventana 
	windowClass.lpszClassName = ClassName;

	//registra la clase de ventana en windows
	if (!RegisterClassExW(&windowClass))
		return false;

	m_instance = instance;
	m_classRegistered = true;

	//estilo de la ventana
	constexpr DWORD style =
		WS_OVERLAPPED |
		WS_CAPTION |
		WS_SYSMENU |
		WS_MINIMIZEBOX;

	//define el tamaño del area dinterna de la ventana
	RECT rectangle{
		0,
		0,
		static_cast<LONG>(clientWidth),
		static_cast<LONG>(clientWidth)
	};

	//ajusta el tamaño total de la ventana
	if (!AdjustWindowRect(&rectangle, style, FALSE))
	{
		Destroy();
		return false;
	}

	const int ouerWidth = rectangle.right - rectangle.left;

	const int outerHeight = rectangle.bottom - rectangle.top;

	//crea la ventana de Windows
	m_handle = CreateWindowExW(
		0,
		ClassName,
		tittle,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		ouerWidth,
		outerHeight,
		nullptr,
		nullptr,
		instance,
		this
	);

	//si no se pudo crear elimina los recursos que se hayan creado
	if (!m_handle)
	{
		Destroy();
		return false;
	}
	return true;
}

void Window::Show(int showCommand) noexcept
{
	if (m_handle)
	{
		ShowWindow(m_handle, showCommand);
		UpdateWindow(m_handle);
	}
}

void Window::Destroy() noexcept
{
	//verifica si existe la ventana y  la destruye
	if (m_handle)
	{
		DestroyWindow(m_handle);
		m_handle = nullptr;
	}


	if (m_classRegistered)
	{
		UnregisterClassW(ClassName, m_instance);
		m_classRegistered = false;
	}

	m_instance = nullptr;
}

bool Window::ProcessMessages() noexcept
{
	MSG message{};

	//obtiene y procesa todos los mensajes pendientes
	while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
	{
		if (message.message == WM_QUIT)
			return false;

		//traduuce el mensaje antes de enviarlo al procedimeinto
		TranslateMessage(&message);
		//envia el mensaje al windowProdcedure
		DispatchMessageW(&message);
	}
	return true;
}


bool Window::IsMinimized() const noexcept
{
	return m_handle && IsIconic(m_handle);
}

LRESULT CALLBACK Window::WindowProcedure(
	HWND handle,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (message)
	{
	case WM_ERASEBKGND:
		//DirectX limpia el back buffer.
		return 1;



	case WM_DESTROY:
		//indica a windows que la aplicacion debe terminar
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProcW(handle, message, wParam, lParam);
	}
}

