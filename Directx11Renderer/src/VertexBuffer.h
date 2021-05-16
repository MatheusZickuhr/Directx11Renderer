#pragma once

#include "Directx11.h"

class VertexBuffer {

public:

	VertexBuffer(void* data, UINT dataSize, UINT stride);

	VertexBuffer(UINT dataSize, UINT stride);

	void bind();

	void pushData(void* data, UINT dataSize);

	UINT getNumVertices() { return this->numVerts; }

private:

	UINT numVerts;
	UINT stride;
	UINT offset;
	ID3D11Buffer* vertexBuffer;
};