#pragma once

#include <memory>
#include "Camera.h"
#include "Input.h"



class CameraController {

public:

	CameraController(Camera& camera);

	void update(float deltaTime);

private:

	Camera& camera;
	float movementSpeed = 2.5f;
	float mouseSensitivity = 0.1f;
	float xlast = 0;
	float ylast = 0;
	bool isFirstMouseMovement = true;

	void processKeybordInput(float deltaTime);

	void processMouseInput();

};
