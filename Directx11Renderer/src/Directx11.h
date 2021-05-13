#pragma once

#include <memory>
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>

#include "Context.h"

class Directx11 {

public:
	void initialize(HWND hwnd);

	Directx11(Directx11 const&) = delete;

	void operator=(Directx11 const&) = delete;

	static Directx11& getInstance();

	ID3D11Device1* getDevice() { return this->d3d11Device; }

	ID3D11DeviceContext1* getDeviceContext() { return this->d3d11DeviceContext; }

	IDXGISwapChain1* getSwapChain() { return this->d3d11SwapChain; }

	ID3D11RenderTargetView* getRenderTargetView() { return this->d3d11FrameBufferView; }

	ID3D11RenderTargetView*const* getRenderTargetViewPtr() { return &this->d3d11FrameBufferView; }

	Context* getContext() { return this->context.get(); }

private:

	HWND hwnd;
	ID3D11Device1* d3d11Device;
	ID3D11DeviceContext1* d3d11DeviceContext;
	IDXGISwapChain1* d3d11SwapChain;
	ID3D11RenderTargetView* d3d11FrameBufferView;

	std::unique_ptr<Context> context = nullptr;

	Directx11();

	void createDeviceAndContext();

	void createDebugLayer();

	void createSwapChain();

	void createFrameBufferRenderTarget();
};