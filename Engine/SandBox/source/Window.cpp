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

	WNDCLASSEXW windowClass{};
	windowClass.cbSize = sizeof(windowClass);
	windowClass.lpfnWndProc = WindowProcedure;
	windowClass.hInstance = instance;
	windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	windowClass.lpszClassName = ClassName;

	if (!RegisterClassExW(&windowClass))
		return false;

	m_instance = instance;
	m_classRegistered = true;

	constexpr DWORD style =
		WS_OVERLAPPED |
		WS_CAPTION |
		WS_SYSMENU |
		WS_MINIMIZEBOX;

	RECT rectangle{
		0,
		0,
		static_cast<LONG>(clientWidth),
		static_cast<LONG>(clientWidth)
	};

	if (!AdjustWindowRect(&rectangle, style, FALSE))
	{
		Destroy();
		return false;
	}

	const int ouerWidth = rectangle.right - rectangle.left;

	const int outerHeight = rectangle.bottom - rectangle.top;

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

		while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
				return false;

			TranslateMessage(&message);
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
				PostQuitMessage(0);
				return 0;

			default:
				return DefWindowProcW(handle, message, wParam, lParam);
		}
	}
	
	