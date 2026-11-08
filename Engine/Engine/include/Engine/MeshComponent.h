#pragma once
#include "Engine/Prerequisites.h"

/**
 * @class MeshComponent
 * @brief Guarda la forma de un objeto 3D
 *
 * Una malla es como una figura armada con muchos triángulos pequeños.
 * Esta clase almacena todo lo necesario para dibujar esa figura
 * sus puntos, cómo se conectan y dónde está colocada
 */
class
    MeshComponent {
public:

    /**
     * @brief Crea una malla vacía, sin puntos ni conexiones todavía.
     */
    MeshComponent() : m_numVertex(0), m_numIndex(0) {}

public:
    /**
    * @brief Nombre de la malla, para identificarla fácilmente.
    */
    std::string m_name;

    /**
    * @brief Posición, giro y tamaño de la malla respecto a su objeto.
    * Por defecto no mueve, no gira y no cambia el tamaño de nada
    * (es decir, la figura queda tal cual fue creada).
    */
    DirectX::XMFLOAT4X4 m_localTransform = DirectX::XMFLOAT4X4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    std::vector<Vertex> m_vertex;
    std::vector<unsigned int> m_index;

    int m_numVertex;

    int m_numIndex;
};

