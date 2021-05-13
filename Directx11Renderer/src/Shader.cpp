#include "Shader.h"
#include <assert.h>

void checkIfFailed(HRESULT& hResult, ID3DBlob* shaderCompileErrorsBlob) {
	if (FAILED(hResult))
	{
		const char* errorString = NULL;
		if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
			errorString = "Could not compile shader; file not found";
		else if (shaderCompileErrorsBlob) {
			errorString = (const char*)shaderCompileErrorsBlob->GetBufferPointer();
			shaderCompileErrorsBlob->Release();
		}
		MessageBoxA(0, errorString, "Shader Compiler Error", MB_ICONERROR | MB_OK);
		return;
	}
}

Shader::Shader(ShaderType shaderType, const std::wstring& filepath) {
	this->shaderType = shaderType;

	switch (shaderType) {

	case ShaderType::Vertex: {
		ID3DBlob* shaderCompileErrorsBlob;
		HRESULT hResult = D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &this->shaderByteCode, &shaderCompileErrorsBlob);
		checkIfFailed(hResult, shaderCompileErrorsBlob);

		hResult = Directx11::getInstance().getDevice()->CreateVertexShader(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), nullptr, &this->vertexShader);
		assert(SUCCEEDED(hResult));
		break;
	}

	case ShaderType::Fragment: {
		ID3DBlob* psBlob;
		ID3DBlob* shaderCompileErrorsBlob;
		HRESULT hResult = D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &psBlob, &shaderCompileErrorsBlob);
		checkIfFailed(hResult, shaderCompileErrorsBlob);
		hResult = Directx11::getInstance().getDevice()->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &this->pixelShader);
		assert(SUCCEEDED(hResult));
		psBlob->Release();
		break;
	}
	default:
		assert(false);
		break;
	}



}

void Shader::bind() {
	switch (shaderType) {

	case ShaderType::Vertex: {
		Directx11::getInstance().getDeviceContext()->IASetInputLayout(this->inputLayout);
		Directx11::getInstance().getDeviceContext()->VSSetShader(this->vertexShader, nullptr, 0);
		break;
	}

	case ShaderType::Fragment: {
		Directx11::getInstance().getDeviceContext()->PSSetShader(pixelShader, nullptr, 0);
		break;
	}
	default:
		assert(false);
		break;
	}

}

void Shader::addInputLayout(D3D11_INPUT_ELEMENT_DESC inputElementDesc[], UINT numberOfelements) {

	HRESULT hResult = Directx11::getInstance().getDevice()->CreateInputLayout(
		inputElementDesc,
		numberOfelements,
		this->shaderByteCode->GetBufferPointer(),
		this->shaderByteCode->GetBufferSize(),
		&this->inputLayout);

	assert(SUCCEEDED(hResult));
	this->shaderByteCode->Release();
}

void Shader::addInputLayout(std::vector<InputElement> inputElements) {
	const UINT numberOfElements = inputElements.size();
	D3D11_INPUT_ELEMENT_DESC* inputElementDesc = new D3D11_INPUT_ELEMENT_DESC[numberOfElements];
	D3D11_INPUT_ELEMENT_DESC* inputElementDescBegin = inputElementDesc;

	for (auto& inputElement : inputElements) {
		inputElementDesc->SemanticName = inputElement.name.c_str();
		inputElementDesc->SemanticIndex = 0;
		inputElementDesc->InputSlot = 0;
		inputElementDesc->AlignedByteOffset = inputElement.offset;
		inputElementDesc->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputElementDesc->InstanceDataStepRate = 0;

		switch (inputElement.inputElementType) {
		case InputElementType::Float: 
			inputElementDesc->Format = DXGI_FORMAT_R32_FLOAT;
			break;
		case InputElementType::Vec2: 
			inputElementDesc->Format = DXGI_FORMAT_R32G32_FLOAT;
			break;
		case InputElementType::Vec3:
			inputElementDesc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		case InputElementType::Vec4:
			inputElementDesc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		default:
			assert(false);
			break;
		}

		inputElementDesc++;
	}

	HRESULT hResult = Directx11::getInstance().getDevice()->CreateInputLayout(
		inputElementDescBegin,
		numberOfElements,
		this->shaderByteCode->GetBufferPointer(),
		this->shaderByteCode->GetBufferSize(),
		&this->inputLayout);

	delete[] inputElementDescBegin;
}
