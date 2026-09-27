/**
 * @brief Guarda la matriz que se utiliza para transformar
 *los vértices del objeto
 * Esta información viene desde C++ y combina la posicin
 * del objeto, la cámara y la perspectiva
 */
cbuffer TransformBuffer : register(b0)
{
    float4x4 worldViewProjection;
};

//Informacion que recibe el vertexShader
//Contiene la posicion del vertice y su color
struct VSInput
{
    float3 position : POSITION;
    float4 color : COLOR;
};

/**
 * @brief Información que sale del Vertex Shader
 *y pasa al Pixel Shader
 *aqui guardamos la nueva posicion del vertice
 * y su color.
 */
struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

/**
 * @brief Vertex Shader.
 *
 * Recibe cada vertice del cubo y transforma su posicionn
 * utilizando la matriz worldViewProjection
 * despues conserva el color del vertice para pasarlo
 * al pixel shader
 * @param input Información del vértice que estamos procesando
 */
PSInput VSMain(VSInput input)
{
    PSInput output;
    output.position = mul(float4(input.position, 1.0f), worldViewProjection);
    output.color = input.color;
    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}