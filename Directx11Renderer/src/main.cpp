#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>
#include <iostream>

#include "Directx11.h"

#include "Shader.h"
#include "VertexBuffer.h"
#include <vector>

#include "Input.h"
#include "Camera.h"
#include "CameraController.h"
#include "ConstantBuffer.h"

#include <glm/glm.hpp>

#include <assert.h>




int main() {

	Camera camera{ {0.0f, 0.0f, 6.0f} };

	CameraController cameraControler{ camera };

	const int width = 800, height = 600;

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	auto window = glfwCreateWindow(width, height, "Directx11Renderer", nullptr, nullptr);

	if (!window) {
		std::cout << "Failed to create window" << std::endl;
		return -1;
	}

	Input::init(window);

	auto hMainWnd = glfwGetWin32Window(window);

	Directx11::getInstance().initialize(hMainWnd);

	struct VS_CONSTANT_BUFFER_DATA {
		glm::mat4 cameraViewProjection;
	};

	VS_CONSTANT_BUFFER_DATA constbufferData{ camera.getViewProjectionMatrix(width, height) };
	ConstantBuffer constBuffer { &constbufferData, sizeof(VS_CONSTANT_BUFFER_DATA) };

	struct Vertex {
		float position[2];
		float color[4];
	};

	// Create Vertex Shader

	Shader vertexShader{ ShaderType::Vertex, L"shaders.hlsl" };

	// Create Pixel Shader
	Shader pixelShader{ ShaderType::Fragment, L"shaders.hlsl" };

	// Create Input Layout
	vertexShader.addInputLayout({
		{InputElementType::Vec2, "POS", offsetof(Vertex, position)},
		{InputElementType::Vec4, "COL", offsetof(Vertex, color)}
		});

	// Create Vertex Buffer
	Vertex vertexData[] = { // x, y, r, g, b, a
		{ {0.0f,  0.5f}, {0.f, 1.f, 0.f, 1.f} },
		{ {0.5f, -0.5f}, {1.f, 0.f, 0.f, 1.f} },
		{ {-0.5f, -0.5f}, {0.f, 0.f, 1.f, 1.f} }
	};
	VertexBuffer vertexBuffer{ vertexData, sizeof(vertexData), sizeof(Vertex) };

	Context* context = Directx11::getInstance().getContext();

	float lastTime = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		cameraControler.update(deltaTime);

		VS_CONSTANT_BUFFER_DATA newConstBufferData {camera.getViewProjectionMatrix(width, height )};
		constBuffer.updateData(&newConstBufferData);


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

