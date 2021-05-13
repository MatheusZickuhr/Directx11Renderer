#pragma once

#include <string>
#include <vector>

#include "Directx11.h"

enum class ShaderType { Vertex, Fragment };

enum class InputElementType { Float, Vec2, Vec3, Vec4 };

struct InputElement {
	InputElementType inputElementType;
	std::string name;
	UINT offset;
};

class Shader {

public:
	Shader(ShaderType, const std::wstring&);

	void bind();

	void addInputLayout(D3D11_INPUT_ELEMENT_DESC inputElementDesc[], UINT numberOfelements);

	void addInputLayout(std::vector<InputElement> inputElements);

private:

	ShaderType shaderType;

	ID3D11VertexShader* vertexShader;
	ID3DBlob* shaderByteCode;
	ID3D11InputLayout* inputLayout;

	ID3D11PixelShader* pixelShader;
};