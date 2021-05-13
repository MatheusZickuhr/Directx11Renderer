#include "Context.h"

Context::Context(HWND hwnd, ID3D11DeviceContext1* d3d11DeviceContext, ID3D11RenderTargetView* d3d11FrameBufferView) :
    hwnd(hwnd),
    d3d11DeviceContext(d3d11DeviceContext) {
    this->d3d11FrameBufferView = d3d11FrameBufferView;
}

void Context::setViewPortToWindowDimensions() {
    RECT winRect;
    GetClientRect(this->hwnd, &winRect);
    D3D11_VIEWPORT viewport = { 0.0f, 0.0f, (FLOAT)(winRect.right - winRect.left), (FLOAT)(winRect.bottom - winRect.top), 0.0f, 1.0f };
    d3d11DeviceContext->RSSetViewports(1, &viewport);
}

void Context::clearRenderTargetView(float color[4]) {
    d3d11DeviceContext->ClearRenderTargetView(this->d3d11FrameBufferView, color);
}

void Context::setRenderTargetView() {

    d3d11DeviceContext->OMSetRenderTargets(1, &this->d3d11FrameBufferView, nullptr);
}

void Context::draw(UINT numberOfVertices) {
    d3d11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    d3d11DeviceContext->Draw(numberOfVertices, 0);
}
