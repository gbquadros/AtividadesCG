#include "Mesh.h"
#include "stb_image.h"
#include "Bezier.h"

void Mesh::initialSceneConfig(string fileName, glm::vec3 position, float scale, float angle, string axis, vector<glm::vec3> controlPoints) {
	this->fileName = fileName;
	this->position = position;
	this->scale = glm::vec3(scale, scale, scale);
	this->angle = angle;
	this->controlPoints = controlPoints;

	if (axis == "X")
	{
		this->axis = glm::vec3(1.0, 0.0, 0.0);
	}
	else if (axis == "Y")
	{
		this->axis = glm::vec3(0.0, 1.0, 0.0);
	}
	else if (axis == "Z")
	{
		this->axis = glm::vec3(0.0, 0.0, 1.0);
	}
}

void Mesh::initialize(Shader* shader)
{
	this->shader = shader;

	loadOBJ();
	loadMTL();
	loadTexture();
	setupSprite();

	if (controlPoints.size() > 0) {
		Bezier bezier;
		this->bezier = bezier;
		this->bezier.setControlPoints(controlPoints);
		this->bezier.setShader(shader);
		this->bezier.generateCurve(100);
		nbCurvePoints = this->bezier.getNbCurvePoints();
	}
}

void Mesh::updatePosition(glm::vec3 position) {
	this->position = position;
}

void Mesh::update()
{
	if (controlPoints.size() > 0) {
		glm::vec3 pointOnCurve = bezier.getPointOnCurve(curvePointIndex);
		this->position = pointOnCurve;
		curvePointIndex = (curvePointIndex + 1) % nbCurvePoints;
	}

	glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(angle), axis);
	model = glm::scale(model, scale);
	shader->setMat4("model", glm::value_ptr(model));
}

void Mesh::draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBindVertexArray(VAO);
	shader->setVec3("ka", ka[0], ka[1], ka[2]);
	shader->setFloat("kd", kd);
	shader->setVec3("ks", ks[0], ks[1], ks[2]);
	shader->setFloat("q", ns);
	glDrawArrays(GL_TRIANGLES, 0, positions.size() / 3);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::deleteVertexArray() {
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::loadOBJ()
{
	vector<glm::vec3> vertexIndices;
	vector<glm::vec2> textureIndices;
	vector<glm::vec3> normalIndices;

	ifstream file("../objects/" + fileName);
	if (!file.is_open())
	{
		cerr << "Failed to open file: " << fileName << endl;
		return;
	}

	string line;
	while (getline(file, line))
	{
		istringstream iss(line);
		string prefix;
		iss >> prefix;

		if (prefix == "mtllib")
		{
			iss >> mtlFilePath;
		}
		else if (prefix == "v")
		{
			float x, y, z;
			iss >> x >> y >> z;
			vertexIndices.push_back(glm::vec3(x, y, z));
		}
		else if (prefix == "vt")
		{
			float u, v;
			iss >> u >> v;
			textureIndices.push_back(glm::vec2(u, v));
		}
		else if (prefix == "vn")
		{
			float x, y, z;
			iss >> x >> y >> z;
			normalIndices.push_back(glm::vec3(x, y, z));
		}
		else if (prefix == "f")
		{
			string v1, v2, v3;
			iss >> v1 >> v2 >> v3;

			glm::ivec3 vIndices, tIndices, nIndices;
			istringstream(v1.substr(0, v1.find('/'))) >> vIndices.x;
			istringstream(v1.substr(v1.find('/') + 1, v1.rfind('/') - v1.find('/') - 1)) >> tIndices.x;
			istringstream(v1.substr(v1.rfind('/') + 1)) >> nIndices.x;
			istringstream(v2.substr(0, v2.find('/'))) >> vIndices.y;
			istringstream(v2.substr(v2.find('/') + 1, v2.rfind('/') - v2.find('/') - 1)) >> tIndices.y;
			istringstream(v2.substr(v2.rfind('/') + 1)) >> nIndices.y;
			istringstream(v3.substr(0, v3.find('/'))) >> vIndices.z;
			istringstream(v3.substr(v3.find('/') + 1, v3.rfind('/') - v3.find('/') - 1)) >> tIndices.z;
			istringstream(v3.substr(v3.rfind('/') + 1)) >> nIndices.z;

			for (int i = 0; i < 3; i++)
			{
				const glm::vec3& vertex = vertexIndices[vIndices[i] - 1];
				const glm::vec2& texture = textureIndices[tIndices[i] - 1];
				const glm::vec3& normal = normalIndices[nIndices[i] - 1];

				positions.push_back(vertex.x);
				positions.push_back(vertex.y);
				positions.push_back(vertex.z);
				textureCoords.push_back(texture.x);
				textureCoords.push_back(texture.y);
				normals.push_back(normal.x);
				normals.push_back(normal.y);
				normals.push_back(normal.z);
			}
		}
	}

	file.close();
}

void Mesh::loadMTL()
{
	string line, readValue;
	ifstream mtlFile("../objects/" + mtlFilePath);

	while (!mtlFile.eof())
	{
		getline(mtlFile, line);

		istringstream iss(line);

		if (line.find("map_Kd") == 0)
		{
			iss >> readValue >> textureFilePath;
		}
		else if (line.find("Ka") == 0)
		{
			float ka1, ka2, ka3;
			iss >> readValue >> ka1 >> ka2 >> ka3;
			ka.push_back(ka1);
			ka.push_back(ka2);
			ka.push_back(ka3);
		}
		else if (line.find("Ks") == 0)
		{
			float ks1, ks2, ks3;
			iss >> readValue >> ks1 >> ks2 >> ks3;
			ks.push_back(ks1);
			ks.push_back(ks2);
			ks.push_back(ks3);
		}
		else if (line.find("Ns") == 0)
		{
			iss >> readValue >> ns;
		}
	}

	if (ka.size() == 0) {
		ka.push_back(1.0f);
		ka.push_back(1.0f);
		ka.push_back(1.0f);
	}

	if (ks.size() == 0) {
		ks.push_back(0.5f);
		ks.push_back(0.5f);
		ks.push_back(0.5f);
	}

	mtlFile.close();
}

void Mesh::loadTexture()
{
	string path = "../textures/" + textureFilePath;
	GLuint texID;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	textureID = texID;
}

void Mesh::setupSprite()
{
	GLuint VAO, VBO[3];

	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GLfloat), positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(GLfloat), textureCoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);

	this->VAO = VAO;
}

void Mesh::scaleDown() {
	scale -= 0.2f;
}

void Mesh::scaleUp() {
	scale += 0.2f;
}

void Mesh::increaseAngle() {
	angle += 5.0;
}

void Mesh::decreaseAngle() {
	angle -= 5.0;
}

void Mesh::rotateX() {
	axis = glm::vec3(1.0, 0.0, 0.0);
}

void Mesh::rotateY() {
	axis = glm::vec3(0.0, 1.0, 0.0);
}

void Mesh::rotateZ() {
	axis = glm::vec3(0.0, 0.0, 1.0);
}

void Mesh::translateX(float distance) {
	position.x += distance;
}

void Mesh::translateY(float distance) {
	position.y += distance;
}

void Mesh::translateZ(float distance) {
	position.z += distance;
}