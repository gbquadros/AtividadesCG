#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <thread>
using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Hermite.h"
#include "Bezier.h"
#include "CatmullRom.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void readSceneConfig(string path);

const GLuint WIDTH = 800, HEIGHT = 600;
glm::vec3 cameraFrontInitial, cameraPosInitial, cameraUpInitial, lightPos, lightColor;
vector<Mesh> sceneObjects;
int selectedObject = 0;
Camera camera;

int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "VISUALIZADOR DE CENAS 3D", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	Shader shader("../shaders/sprite.vs", "../shaders/sprite.fs");

	readSceneConfig("../config/cena-config.txt");

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glUseProgram(shader.ID);

	camera.initialize(&shader, width, height, cameraPosInitial, cameraFrontInitial, cameraUpInitial);

	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i].initialize(&shader);
	}

	shader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
	shader.setVec3("lightColor", lightColor.x, lightColor.y, lightColor.z);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		camera.update();

		for (int i = 0; i < sceneObjects.size(); i++)
		{
			sceneObjects[i].update();
			sceneObjects[i].draw();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		glfwSwapBuffers(window);
	}

	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i].deleteVertexArray();
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	camera.move(window, key, action);

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		selectedObject++;

		if (selectedObject >= sceneObjects.size()) {
			selectedObject = 0;
		}
	} 
	else if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) 
	{
		sceneObjects[selectedObject].scaleDown();
	}
	else if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) 
	{
		sceneObjects[selectedObject].scaleUp();
	}
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		sceneObjects[selectedObject].translateY(0.1f);
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		sceneObjects[selectedObject].translateY(-0.1f);
	}
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		sceneObjects[selectedObject].translateZ(0.1f);
	}
	else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		sceneObjects[selectedObject].translateZ(-0.1f);
	}
	else if (key == GLFW_KEY_8 && action == GLFW_PRESS)
	{
		sceneObjects[selectedObject].decreaseAngle();
	}
	else if (key == GLFW_KEY_9 && action == GLFW_PRESS)
	{
		sceneObjects[selectedObject].increaseAngle();
	}
	else if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		sceneObjects[selectedObject].rotateX();
	}
	else if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		sceneObjects[selectedObject].rotateY();
	}
	else if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		sceneObjects[selectedObject].rotateZ();
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		sceneObjects[selectedObject].translateX(0.1f);
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		sceneObjects[selectedObject].translateX(-0.1f);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.rotate(window, xpos, ypos);
}

void readSceneConfig(string path) {
	vector<string> objectsFileName, objectsAxis;
	vector<glm::vec3> objectsPosition;
	vector<vector<glm::vec3>> objectsControlPoints;
	int objectIndex = 0;
	vector<float> objectsScale, objectsAngle;

	string line;
	ifstream configFile(path);

	while (getline(configFile, line))
	{
		istringstream iss(line);
		string prefix;
		iss >> prefix;

		if (prefix == "cameraPos") 
		{
			float x, y, z;
			iss >> x >> y >> z;
			cameraPosInitial = glm::vec3(x, y, z);
		}
		else if (prefix == "cameraFront") 
		{
			float x, y, z;
			iss >> x >> y >> z;
			cameraFrontInitial = glm::vec3(x, y, z);
		}
		else if (prefix == "cameraUp")
		{
			float x, y, z;
			iss >> x >> y >> z;
			cameraUpInitial = glm::vec3(x, y, z);
		}
		else if (prefix == "lightPos")
		{
			float x, y, z;
			iss >> x >> y >> z;
			lightPos = glm::vec3(x, y, z);
		}
		else if (prefix == "lightColor")
		{
			float x, y, z;
			iss >> x >> y >> z;
			lightColor = glm::vec3(x, y, z);
		}
		else if (prefix == "fileName")
		{
			string fileName;
			iss >> fileName;
			objectsFileName.push_back(fileName);
		}
		else if (prefix == "position")
		{
			float x, y, z;
			iss >> x >> y >> z;
			objectsPosition.push_back(glm::vec3(x, y, z));
		}
		else if (prefix == "scale")
		{
			float scale;
			iss >> scale;
			objectsScale.push_back(scale);
		}
		else if (prefix == "angle")
		{
			float angle;
			iss >> angle;
			objectsAngle.push_back(angle);
		}
		else if (prefix == "axis")
		{
			string axis;
			iss >> axis;
			objectsAxis.push_back(axis);
		}
		else if (prefix == "startCurve")
		{
			vector<glm::vec3> controlPoints;
			objectsControlPoints.push_back(controlPoints);
		}
		else if (prefix == "curvePoint")
		{
			float x, y, z;
			iss >> x >> y >> z;
			objectsControlPoints[objectIndex].push_back(glm::vec3(x, y, z));
		}
		else if (prefix == "endCurve")
		{
			objectIndex++;
		}
		else if (prefix == "noCurve")
		{
			vector<glm::vec3> emptyControlPoints;
			objectsControlPoints.push_back(emptyControlPoints);
			objectIndex++;
		}
	}

	configFile.close();

	for (int i = 0; i < objectsFileName.size(); i++)
	{
		Mesh object;
		object.initialSceneConfig(objectsFileName[i], objectsPosition[i], objectsScale[i], objectsAngle[i], objectsAxis[i], objectsControlPoints[i]);
		sceneObjects.push_back(object);
	}
}