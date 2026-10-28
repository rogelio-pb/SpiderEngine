#pragma once
#include "API.h"
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
#define SAFE_RELEASE(x) if(x != nullptr) x->Release(); x = nullptr;

#define MESSAGE( classObj, method, state )   \
{                                            \
   std::wostringstream os_;                  \
   os_ << classObj << "::" << method << " : " << "[CREATION OF RESOURCE " << ": " << state << "] \n"; \
   OutputDebugStringW( os_.str().c_str() );  \
}

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

template<typename T>
void
SafeRelease(T * &object) noexcept {
	if (object != nullptr)
	{
		object->Release();
		object = nullptr;
	}
}
/**
 * @brief Funciones principales que forman parte de la API del Engine
 * Estas funciones permiten que codigo externo pueda
 * inicializar, utilizar y cerrar el Engine
 */
extern "C" {
	ENGINE_API bool
		engine_Initialize(HWND hwnd, int width, int height) noexcept;

	ENGINE_API void
		Engine_Render() noexcept;

	ENGINE_API void
		Engine_Render() noexcept;

	ENGINE_API void
		Engine_Shutdown() noexcept;
}
