#pragma once
#include "Directx11.h"
#include <assert.h>

class ConstantBuffer {

public:

	ConstantBuffer(void* constantBufferData, UINT sizeOfConstantBufferData);

	void updateData(void* constantBufferNewData);

private:

	ID3D11Buffer* constantBuffer;
};