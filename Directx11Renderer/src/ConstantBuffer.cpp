#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(void* constantBufferData, UINT sizeOfConstantBufferData) {
	this->constantBuffer = NULL;

	// Fill in a buffer description.
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeOfConstantBufferData;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = constantBufferData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the buffer.
	HRESULT hr = Directx11::getInstance().getDevice()->CreateBuffer(&cbDesc, &InitData, &this->constantBuffer);

	if (FAILED(hr))
		assert(false);

	// Set the buffer.
	Directx11::getInstance().getDeviceContext()->VSSetConstantBuffers(0, 1, &this->constantBuffer);
}

void ConstantBuffer::updateData(void* constantBufferNewData) {
	Directx11::getInstance().getDeviceContext()->UpdateSubresource(this->constantBuffer, 0, 0, constantBufferNewData, 0, 0);
	Directx11::getInstance().getDeviceContext()->VSSetConstantBuffers(0, 1, &this->constantBuffer);
}
