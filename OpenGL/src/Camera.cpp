#include "Camera.h"
#include <iostream>
#include "config.h"

Camera::Camera(float fov, glm::vec3 camerapos)
	:Fov(fov), cameraPos(camerapos), cameraFront(glm::vec3(0.0f, 0.0f, 1.0f)), cameraUp(glm::vec3(0, 1, 0))
{
	//perspectiveProjMat = glm::ortho(-225.0f, 225.0f, -400.0f, 400.0f, 0.1f, 1000.0f);//正交投影矩阵
#ifdef Show
	perspectiveProjMat = glm::perspective(glm::radians(Fov), 16.0f / 9.0f, 0.1f, 1000.0f);//透视投影矩阵
#else
	//perspectiveProjMat = glm::perspective(glm::radians(Fov), 3.0f / 4.0f, 0.1f, 1000.0f);//透视投影矩阵
	perspectiveProjMat = glm::perspective(glm::radians(Fov), 16.0f / 9.0f, 0.1f, 10000.0f);//透视投影矩阵
#endif // Show
	
	//lastX = 450.0f / 2;
	//lastY = 800.0f / 2;
	lastX = 1920.0f / 2;
	lastY = 1080.0f / 2;
	yaw = pitch = 0;
}

Camera::~Camera()
{

}



void Camera::SetKeyboardInput(glm::vec2 input)
{
	cameraPos += input.x * cameraSpeed * deltaTime * cameraFront;
	cameraPos += input.y * cameraSpeed * deltaTime * glm::normalize(glm::cross(cameraFront, cameraUp));
}

void Camera::SetMouseInput(double xpos, double ypos)
{
	if(!enableCameraRotation)
		return;
	
	if (abs(lastX-xpos) >1000 || abs(lastY - ypos) > 1000)
	{
		lastX = xpos;
		lastY = ypos;
		//firstMouse = false;
	}


	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xoffset *= rotateSensitivity;
	yoffset *= rotateSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraFront = glm::normalize(front);
}


void Camera::UpdateDeltaTime()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void Camera::display()
{
	std::cout << "camerafront.x: " << cameraFront.x << ", camerafront.y: " << cameraFront.y << ", camerafront.z: " << cameraFront.z << std::endl;
	std::cout << &cameraFront << std::endl;
	std::cout << std::endl;
}
