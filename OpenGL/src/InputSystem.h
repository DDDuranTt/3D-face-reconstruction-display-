#pragma once



#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

#include "Camera.h"


class InputSystem
{
private:

	static double mouseX;
	static double mouseY;

	double lastMouseX,lastMouseY;
	GLFWwindow *window;
	Camera* boundCamera;

	bool enableCursor;
public:
	InputSystem(GLFWwindow* _window, Camera* camera);
	~InputSystem();


	void InputDetect();

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

};


