#pragma once

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include <vector>
#include "Bezier.h"

class Mesh
{
public:
	Mesh() {}
	~Mesh() {}
	void initialSceneConfig(string fileName, glm::vec3 position, float scale, float angle, string axis, vector<glm::vec3> controlPoints);
	void initialize(Shader* shader);
	void update();
	void draw();
	void updatePosition(glm::vec3 position);
	void loadOBJ();
	void loadMTL();
	void loadTexture();
	void setupSprite();
	void deleteVertexArray();
	void scaleDown();
	void scaleUp();
	void increaseAngle();
	void decreaseAngle();
	void rotateX();
	void rotateY();
	void rotateZ();
	void translateX(float distance);
	void translateY(float distance);
	void translateZ(float distance);

protected:
	string fileName = "", mtlFilePath = "", textureFilePath = "";

	vector<GLfloat> positions, textureCoords, normals, ka, ks;
	float ns = 250.0f, kd = 0.5f;
	vector <glm::vec3> controlPoints;

	GLuint VAO; //Identificador do VAO

	//Informações sobre as transformações a serem aplicadas no objeto
	glm::vec3 position;
	glm::vec3 scale;
	float angle;
	glm::vec3 axis;

	//Referência do shader
	Shader* shader;

	GLuint textureID;
	Bezier bezier;
	int nbCurvePoints;
	int curvePointIndex = 0;
};

