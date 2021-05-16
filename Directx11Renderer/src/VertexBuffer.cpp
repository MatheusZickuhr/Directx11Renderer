#include "VertexBuffer.h"
#include <assert.h>
VertexBuffer::VertexBuffer(void* data, UINT dataSize, UINT stride) {

	this->stride = stride;

	this->numVerts = dataSize / this->stride;
	this->offset = 0;

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = dataSize;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubresourceData = { data };

	HRESULT hResult = Directx11::getInstance().getDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &this->vertexBuffer);
	assert(SUCCEEDED(hResult));

}

VertexBuffer::VertexBuffer(UINT dataSize, UINT stride) {

	this->stride = stride;

	this->numVerts = dataSize / this->stride;
	this->offset = 0;

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = dataSize;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hResult = Directx11::getInstance().getDevice()->CreateBuffer(&vertexBufferDesc, nullptr, &this->vertexBuffer);
	assert(SUCCEEDED(hResult));
}

void VertexBuffer::bind() {
	Directx11::getInstance().getDeviceContext()->IASetVertexBuffers(0, 1, &this->vertexBuffer, &this->stride, &this->offset);
}

void VertexBuffer::pushData(void* data, UINT dataSize) {
	D3D11_MAPPED_SUBRESOURCE resource;
	Directx11::getInstance().getDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, dataSize);
	Directx11::getInstance().getDeviceContext()->Unmap(this->vertexBuffer, 0);
}
