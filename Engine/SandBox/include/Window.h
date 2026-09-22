#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

class Window final {
public:
	Window() = default;
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	bool
		Create(HINSTANCE instance, const wchar_t* tittle,
			UINT clientWidth, UINT clientHeight) noexcept;

	void
		Show(int showCommand) noexcept;

	void
		Destroy() noexcept;

	//Devuelve false cuando se recibe WM_QUIT.
	bool
		ProcessMessages() noexcept;

	HWND
		GetHandle() const noexcept;

	bool
		IsMinimized() const noexcept;

private:
	static LRESULT CALLBACK
		WindowProcedure(HWND handle, UINT message,
			WPARAM aParam, LPARAM lParam);

	static constexpr const wchar_t* ClassName =
		L"SpiderEngineWindow";

	HINSTANCE m_instance = nullptr;
	HWND m_handle = nullptr;
	bool m_classRegistered = false;
};
