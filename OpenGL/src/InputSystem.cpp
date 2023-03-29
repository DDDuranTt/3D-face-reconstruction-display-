#include "InputSystem.h"

double InputSystem::mouseX;
double InputSystem::mouseY;

InputSystem::InputSystem(GLFWwindow* _window, Camera* camera)
	:window(_window), boundCamera(camera), lastMouseX(0.0f), lastMouseY(0.0f), enableCursor(false)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	mouseX = mouseY = 0.0f;
}

InputSystem::~InputSystem()
{

}

void InputSystem::InputDetect()
{
	if (enableCursor = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	glm::vec2 keyboardInput(glfwGetKey(window, GLFW_KEY_W) + -glfwGetKey(window, GLFW_KEY_S), -glfwGetKey(window, GLFW_KEY_A) + glfwGetKey(window, GLFW_KEY_D));
	if (keyboardInput != glm::vec2(0))
		boundCamera->SetKeyboardInput(keyboardInput);

	if ((mouseX != lastMouseX || mouseY != lastMouseY) && !enableCursor) {
		boundCamera->SetMouseInput(mouseX, mouseY);
		lastMouseX = mouseX;
		lastMouseY = mouseY;
	}

}

void InputSystem::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouseX = xpos;
	mouseY = ypos;
}



