#include "Directx11.h"
#include <assert.h>

Directx11::Directx11() {}

void Directx11::initialize(HWND hwnd) {
    this->hwnd = hwnd;

	this->createDeviceAndContext();
    this->createDebugLayer();
    this->createSwapChain();
    this->createFrameBufferRenderTarget();

    this->context = std::make_unique<Context>(this->hwnd, this->d3d11DeviceContext, d3d11FrameBufferView);
}

Directx11& Directx11::getInstance() {
	static Directx11 instance;
	return instance;
}

void Directx11::createDeviceAndContext() {
    ID3D11Device* baseDevice;
    ID3D11DeviceContext* baseDeviceContext;
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG)
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hResult = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE,
        0, creationFlags,
        featureLevels, ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION, &baseDevice,
        0, &baseDeviceContext);
    if (FAILED(hResult)) {
        MessageBoxA(0, "D3D11CreateDevice() failed", "Fatal Error", MB_OK);
        //return GetLastError();
    }

    // Get 1.1 interface of D3D11 Device and Context
    hResult = baseDevice->QueryInterface(__uuidof(ID3D11Device1), (void**)&this->d3d11Device);
    assert(SUCCEEDED(hResult));
    baseDevice->Release();

    hResult = baseDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&this->d3d11DeviceContext);
    assert(SUCCEEDED(hResult));
    baseDeviceContext->Release();

}

void Directx11::createDebugLayer() {
#ifdef DEBUG
    // Set up debug layer to break on D3D11 errors
    ID3D11Debug* d3dDebug = nullptr;
    d3d11Device->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3dDebug);
    if (d3dDebug)
    {
        ID3D11InfoQueue* d3dInfoQueue = nullptr;
        if (SUCCEEDED(d3dDebug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&d3dInfoQueue)))
        {
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
            d3dInfoQueue->Release();
        }
        d3dDebug->Release();
    }
#endif
}

void Directx11::createSwapChain() {
    // Get DXGI Factory (needed to create Swap Chain)
    IDXGIFactory2* dxgiFactory;
    {
        IDXGIDevice1* dxgiDevice;
        HRESULT hResult = d3d11Device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice);
        assert(SUCCEEDED(hResult));

        IDXGIAdapter* dxgiAdapter;
        hResult = dxgiDevice->GetAdapter(&dxgiAdapter);
        assert(SUCCEEDED(hResult));
        dxgiDevice->Release();

        DXGI_ADAPTER_DESC adapterDesc;
        dxgiAdapter->GetDesc(&adapterDesc);

        OutputDebugStringA("Graphics Device: ");
        OutputDebugStringW(adapterDesc.Description);

        hResult = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory);
        assert(SUCCEEDED(hResult));
        dxgiAdapter->Release();
    }

    DXGI_SWAP_CHAIN_DESC1 d3d11SwapChainDesc = {};
    d3d11SwapChainDesc.Width = 0; // use window width
    d3d11SwapChainDesc.Height = 0; // use window height
    d3d11SwapChainDesc.BufferCount = 2;
    d3d11SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    d3d11SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    d3d11SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    d3d11SwapChainDesc.SampleDesc.Count = 1;

    HRESULT hResult = dxgiFactory->CreateSwapChainForHwnd(d3d11Device, hwnd, &d3d11SwapChainDesc, 0, 0, &d3d11SwapChain);
    assert(SUCCEEDED(hResult));

    dxgiFactory->Release();
}

void Directx11::createFrameBufferRenderTarget() {
    ID3D11Texture2D* d3d11FrameBuffer;
    HRESULT hResult = this->d3d11SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3d11FrameBuffer);
    assert(SUCCEEDED(hResult));

    hResult = d3d11Device->CreateRenderTargetView(d3d11FrameBuffer, 0, &d3d11FrameBufferView);
    assert(SUCCEEDED(hResult));
    d3d11FrameBuffer->Release();
}
