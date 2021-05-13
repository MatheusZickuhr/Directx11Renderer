#pragma once

#include <string>
#include <d3d11_1.h>
#include <d3dcompiler.h>

#include "Directx11.h"

enum class ShaderType { Vertex, Fragment };

class Shader {

public:
	Shader(ShaderType, const std::wstring&);

	void bind();

	void addInputLayout(D3D11_INPUT_ELEMENT_DESC inputElementDesc[], UINT numberOfelements);

private:

	ShaderType shaderType;

	ID3D11VertexShader* vertexShader;
	ID3DBlob* shaderByteCode;
	ID3D11InputLayout* inputLayout;

	ID3D11PixelShader* pixelShader;
};