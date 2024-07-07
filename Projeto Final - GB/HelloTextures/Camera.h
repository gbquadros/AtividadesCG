#pragma once

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


class Camera
{
public:
	Camera() {}
	~Camera() {}
	void initialize(Shader* shader, int width, int height, glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 3.0), glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0), glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0), float sensitivity = 0.05, float pitch = 0.0, float yaw = -90.0);
	void move(GLFWwindow* window, int key, int action);
	void rotate(GLFWwindow* window, double xpos, double ypos);
	void update();

protected:
	Shader* shader;
	bool firstMouse, rotateX, rotateY, rotateZ;
	float lastX, lastY, pitch, yaw;
	float sensitivity;
	glm::vec3 cameraFront, cameraPos, cameraUp;
};

