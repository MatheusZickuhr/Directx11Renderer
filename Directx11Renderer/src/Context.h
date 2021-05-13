#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define UNICODE
#include <windows.h>

#include <d3d11_1.h>

class Context {

public:

	Context(HWND hwnd, ID3D11DeviceContext1* d3d11DeviceContext, ID3D11RenderTargetView* d3d11FrameBufferView);

	void setViewPortToWindowDimensions();

	void clearRenderTargetView(float color[4]);

	void setRenderTargetView();

	void draw(UINT numberOfVertices);

private:
	ID3D11DeviceContext1* d3d11DeviceContext;
	ID3D11RenderTargetView* d3d11FrameBufferView;
	HWND hwnd;
};