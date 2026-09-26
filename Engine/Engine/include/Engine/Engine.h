#pragma once

#include "Prerequisites.h"

/**
 * @brief Esta es la clase principal del motor
 
 * Esta clase se encarga de controlar las partes principales
 * del motor, como iniciar, dibujar y cerrar el programa.
 */
class ENGINE_API
	Engine final {
public:
	//Instancias del motor
	Engine() noexcept;
	~Engine() noexcept;

	//Engine& = delete evita que se pueda copiar el motor
	Engine(const Engine&) = delete;

	//Evita asignar una copia del motor a otro objeto
	Engine& operator=(const Engine&) = delete;

	//Engine&& = delete evita mover el motor a otro objeto
	Engine(const Engine&&) = delete;

	//Evita asignr el motor mediante movimineto
	Engine& operator=(Engine&&) = delete;

	/**
	 * @brief Inicializa el motor
	 * Recibe la ventana donde se mostrara el programa,
	 * además de su ancho y alto
	 * @param nativeWindow Ventana donde se mostrará el motor
	 * @param width Ancho de la ventana
	 * @param height Alto de la ventana
	 * @return true si la inicialización se realizó correctamente,
	 * false si ocurrió algún problem
	 */
	bool Initialize(
		void* nativeWindow,
		std::uint32_t width,
		std::uint32_t height
		) noexcept;

	//La funcion es la que se llama para renderizar
	void Render() noexcept;

	//Esta funcion es cuando dejamos de ejecutar, cieera y libera sus recursos
	void Shutdown() noexcept;

private:
	//Se usa para mantener los detalles internos del motor
	struct Implementation;

	//Se usa para apuntar y/o cceder a las partes internas del motor
	Implementation* m_implementation = nullptr;
};
