#pragma once
#include "Engine/Prerequisites.h"

class
    MeshComponent {
public:

    MeshComponent() : m_numVertex(0), m_numIndex(0) {}

public:
    std::string m_name;

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

