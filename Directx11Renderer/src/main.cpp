#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "dxgi.lib")

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define UNICODE

#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>
#include <iostream>


#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>

#include <assert.h>

#include "Directx11.h"

#include "Shader.h"
#include "VertexBuffer.h"

int main() {

    const int width = 800, height = 600;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    auto window = glfwCreateWindow(width, height, "Directx11Renderer", nullptr, nullptr);

    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        return -1;
    }

    auto hMainWnd = glfwGetWin32Window(window);

    Directx11::getInstance().initialize(hMainWnd);

    // Create Vertex Shader
    
    Shader vertexShader{ ShaderType::Vertex, L"shaders.hlsl" };

    // Create Pixel Shader
    Shader pixelShader{ ShaderType::Fragment, L"shaders.hlsl" };

    // Create Input Layout
    
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    vertexShader.addInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc));

    // Create Vertex Buffer
    float vertexData[] = { // x, y, r, g, b, a
        0.0f,  0.5f, 0.f, 1.f, 0.f, 1.f,
        0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f,
        -0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f
    };
    VertexBuffer vertexBuffer { vertexData, sizeof(vertexData), 6 * sizeof(float) };

    Context* context = Directx11::getInstance().getContext();

    while (!glfwWindowShouldClose(window)) {

        FLOAT backgroundColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        context->clearRenderTargetView(backgroundColor);

        context->setViewPortToWindowDimensions();

        context->setRenderTargetView();

        vertexShader.bind();
        pixelShader.bind();

        vertexBuffer.bind();

        context->draw(vertexBuffer.getNumVertices());

        Directx11::getInstance().getSwapChain()->Present(0, 0);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

