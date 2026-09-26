#pragma once
#if defined(_WIN32) 
/**
* @brief indica que estamos utilizando el DLL ya creado
* estamos utilizando funciones que ya existen dentro de ella.
*/
#if defined(ENGINE_BUILD_DLL)
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
/**
 * @brief Para sistemas que no son Windows
 * En otros sistemas no necesitamos estas instrucciones especiales,
 * por lo que ENGINE_API simplemente queda vacío
 */
#else
#define ENGINE_API
#endif
