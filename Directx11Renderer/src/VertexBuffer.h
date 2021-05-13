#pragma once

#include "Directx11.h"

class VertexBuffer {

public:

	VertexBuffer(void* data, UINT dataSize, UINT stride);

	void bind();

	UINT getNumVertices() { return this->numVerts; }

private:

	UINT numVerts;
	UINT stride;
	UINT offset;
	ID3D11Buffer* vertexBuffer;
};