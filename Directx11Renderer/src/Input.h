#pragma once

#define SCAPE_KEY = GLFW_KEY_ESCAPE


struct GLFWwindow;

class Input {

private:

	static GLFWwindow* window;

public:

	static void init(GLFWwindow* windowInstance);

	static void getCursorPos(double* xpos, double* ypos);

	static bool keyPressed(char key);

};

