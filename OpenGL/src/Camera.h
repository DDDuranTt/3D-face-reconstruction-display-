#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


//static const float SCR_WIDTH = 1920.0f;
//static const float SCR_HEIGHT = 1080.0f;

class Camera
{
private:
	float Fov;
	float yaw, pitch;
	float lastX, lastY;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::mat4 perspectiveProjMat;
public:
	bool firstMouse = true;
	float rotateSensitivity = 0.2f;
	float cameraSpeed = 100.0f;

	bool enableCameraRotation = false;

	Camera(float fov, glm::vec3 camerapos);
	~Camera();

	void SetKeyboardInput(glm::vec2 input);

	void SetMouseInput(double xpos, double ypos);

	void UpdateDeltaTime();

	inline glm::mat4 GetMatrix() { return perspectiveProjMat * glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); }

	void display();
};


