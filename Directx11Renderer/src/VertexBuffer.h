#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include "Directx11.h"

class VertexBuffer {

public:

	VertexBuffer(float data[], UINT dataSize, UINT stride);

	void bind();

	UINT getNumVertices() { return this->numVerts; }

private:

	UINT numVerts;
	UINT stride;
	UINT offset;
	ID3D11Buffer* vertexBuffer;
};