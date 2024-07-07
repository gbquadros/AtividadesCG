#include <iostream>
#include <string>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


// Header função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


// Header das funções
int numero_vertices = 0;
int setupShader();
int setupGeometry();

// Dimensões da janela
const GLuint WIDTH = 1000, HEIGHT = 1000;

//Vertex Shader
const GLchar* vertexShaderSource = "#version 450\n"
"layout (location = 0) in vec3 position;\n"
"uniform mat4 model;\n"
"void main()\n"
"{\n"

"gl_Position = model * vec4(position, 1.0);\n"
"}\0";

//Fragment Shader
const GLchar* fragmentShaderSource = "#version 450\n"
"uniform vec4 finalColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = finalColor;\n"
"}\n\0";

bool rotateX = false, rotateY = false, rotateZ = false;


int main()
{
	// Inicialização da GLFW
	glfwInit();


	// Cria a janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello 3D world!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Faze o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros das funções de OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtem as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Define as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compila e gera o executavel o programa de shader
	GLuint shaderID = setupShader();

	// Gera um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();

	glUseProgram(shaderID);

	glm::mat4 model = glm::mat4(1); //matriz identidade;
	GLint modelLoc = glGetUniformLocation(shaderID, "model");
	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(model));

	glEnable(GL_DEPTH_TEST);

	GLint colorLoc = glGetUniformLocation(shaderID, "finalColor");
	glUniform4f(colorLoc, 1.0, 0.0, 0.0, 1.0);

	// Loop da aplicação
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (teclado, mouse) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		float angle = (GLfloat)glfwGetTime();

		model = glm::mat4(1);
		if (rotateX)
		{
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (rotateY)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (rotateZ)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
		}


		glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(model));
		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0,(numero_vertices*3));
		glUniform4f(colorLoc, 0.0, 0.0, 0.0, 1.0);
		
		for (int n = 0; n < numero_vertices; n+=3) {
			glDrawArrays(GL_LINE_LOOP, n, 3);
		}

		glUniform4f(colorLoc, 1.0, 0.0, 0.0, 1.0);

		// Chamada de desenho - drawcall
		// CONTORNO - GL_LINE_LOOP

		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// desaloca os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW e limpa os recursos alocados
	glfwTerminate();
	return 0;
}

// Função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		rotateX = true;
		rotateY = false;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = true;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = false;
		rotateZ = true;
	}



}

// função que compilar um programa de shader
// retorna o identificador do programa de shader
int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERRO::SHADER::VERTEX::NAO::ENCONTRADO\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERRO::SHADER::FRAGMENT::NAO::ENCONTRADO\n" << infoLog << std::endl;
	}
	// faz o link dos shaders e cria o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// tratamento de erros
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERRO::SHADER::PROGRAM::LINK\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	return shaderProgram;
}

// cria os buffers que armazenam a geometria de um triângulo
// retorna o identificador do VAO

int setupGeometry()
{
	
	//leitura do arquivo
	std::vector< glm::vec3 > vertexIndices = {};
	std::vector< glm::vec3 > triangulos = {};
	string line;
	string v, valuesX, valuesY, valuesZ;

	ifstream myfile("suzanneTri.obj");
	while (!myfile.eof())
	{
		getline(myfile, line);
		std::istringstream iss(line);
		if (line[0] == 'v' && line[1] == ' ')		{
			iss >> v >> valuesX >> valuesY >> valuesZ;
			glm::vec3 aux = { std::stof(valuesX), std::stof(valuesY), std::stof(valuesZ) };
			vertexIndices.push_back(aux);
		}

		if(line[0] == 'f' && line[1] == ' ')
		{
			
			string delimiter = " ";
			string delimiter1 = "/";
			int primeira_parte = 0;
			int segunda_parte = 0;
			int terceira_parte = 0;
			auto finish = line.find(delimiter); // gives position of first occurrence of delimiter
			line = line.substr(finish + 1);

			if (finish != line.npos) { // find returned the end of string
				primeira_parte = stoi(line.substr(0, line.find(delimiter1))); // 
			}

			finish = line.find(delimiter); // gives position of first occurrence of delimiter
			line = line.substr(finish + 1);

			if (finish != line.npos) { // find returned the end of string
				segunda_parte = stoi(line.substr(0, line.find(delimiter1))); // 
			}
			
			finish = line.find(delimiter); // gives position of first occurrence of delimiter
			line = line.substr(finish + 1);

			if (finish != line.npos) { // find returned the end of string
				terceira_parte = stoi(line.substr(0, line.find(delimiter1))); // 
			}

			triangulos.push_back(vertexIndices[terceira_parte - 1]);
			triangulos.push_back(vertexIndices[segunda_parte - 1]);
			triangulos.push_back(vertexIndices[primeira_parte-1]);
			

			numero_vertices++;
		}
	}

	GLuint VBO, VAO;

	//Gera o identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer do OpenGl
	glBufferData(GL_ARRAY_BUFFER, triangulos.size() * sizeof(GLfloat)*3, &triangulos[0], GL_STATIC_DRAW);

	//Geração do identificador do VAO
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Para cada atributo do vertice, cria um "AttribPointer", indicando: 
	// Localização no shader *
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	



	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO
	glBindVertexArray(0);

	return VAO;
}

