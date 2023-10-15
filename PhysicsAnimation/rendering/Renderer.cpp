#include "Renderer.h"
#include <iostream>

#define GLM_FORCE_RADIANS
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

void loadShader(GLuint shaderID, const GLchar* shaderSource) {
	glShaderSource(shaderID, 1, &shaderSource, NULL);
	glCompileShader(shaderID);
	//Let’s double check the shader compiled
	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status); //Check for errors
	if (!status) {
		char buffer[512]; glGetShaderInfoLog(shaderID, 512, NULL, buffer);
		std::cout << "Shader Compile Failed. Info:\n\n" << buffer << "\n";
	}
}


const GLchar* vertexSource =
"#version 150 core\n"
"in vec3 position;"
"in vec4 inColor;"
"out vec4 Color;"
"uniform mat4 view;"
"uniform mat4 proj;"
"void main() {"
" Color = inColor;"
" gl_Position = proj * view * vec4(position, 1.0);"
"}";

const GLchar* fragmentSource =
"#version 150 core\n"
"in vec4 Color;"
"out vec4 outColor;"
"void main() {"
" outColor = vec4(Color);" //(Red, Green, Blue, Alpha)
"}";


Renderer::Renderer() : cameraPos(0, 0, 0), cameraScale(1,1,1), cameraDir(0,0,-1)
{
	// Create shader, temporary, should move to seperate class
	// Load and COmpile Shaders
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	loadShader(vertexShader, vertexSource);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	loadShader(fragmentShader, fragmentSource);
	//Join the vertex and fragment shaders together into one program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor"); // set output
	glLinkProgram(shaderProgram); //run the linker

	// Create VBO
	vboContents = std::vector<float>(vboSize);
	for (int i = 0; i < vboSize; i++)
		vboContents[i] = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, vboSize * sizeof(float), &vboContents[0], GL_STATIC_DRAW);

	// Create VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create EBO
	indices = std::vector<unsigned int>(indicesSize);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

	posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), 0);
	//(above params: Attribute, vals/attrib., type, isNormalized, stride, offset)
	glEnableVertexAttribArray(posAttrib); //Mark the attribute’s location as valid
	colAttrib = glGetAttribLocation(shaderProgram, "inColor");
	glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE,
		vertexSize * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(colAttrib);
	glBindVertexArray(0); //Unbind the VAO so we don’t accidentally modify it

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


Renderer::~Renderer()
{
	
}

void Renderer::StartFrame()
{
	currentVboLoc = 0;
	currentIndicesLoc = 0;
}

void Renderer::Render(const Renderable& obj)
{
	int numVertices = obj.NumPoints() * vertexSize;
	int numIndices = obj.NumIndices();

	if (numVertices + currentVboLoc > vboSize) {
		std::cout << "VBO Size Exceeded\n";
		return;
	}

	if (numIndices + currentIndicesLoc > indicesSize) {
		std::cout << "EBO Size Exceeded\n";
		return;
	}

	obj.Render(vboContents, currentVboLoc, indices, currentIndicesLoc);
	currentVboLoc += numVertices;
	currentIndicesLoc += numIndices;
}

void Renderer::SetCamera(const Pos3F& pos, const Pos3F& scale, const Vec3F& dir)
{
	cameraPos = pos;
	cameraScale = scale;
	cameraDir = dir;
}

void Renderer::FinalizeFrame()
{
	

	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	int cameraLocation = glGetUniformLocation(shaderProgram, "CameraPos");
	glUniform3f(cameraLocation, cameraPos.x, cameraPos.y, cameraPos.z);
	int cameraScaleLoc = glGetUniformLocation(shaderProgram, "CameraScale");
	glUniform2f(cameraScaleLoc, cameraScale.x, cameraScale.y);

	glm::mat4 view = glm::lookAt(
		glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z), 
		glm::vec3(cameraPos.x + cameraDir.x, cameraPos.y + cameraDir.y, cameraPos.z + cameraDir.z), 
		glm::vec3(0, 1, 0)
	);
	GLint uView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective(3.14f / 4, 1.0f, 0.1f, 10.0f);
	GLint uProj = glGetUniformLocation(shaderProgram, "proj");
	glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(proj));

	glBufferData(GL_ARRAY_BUFFER, currentVboLoc * sizeof(float), &vboContents[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentIndicesLoc * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, currentIndicesLoc, GL_UNSIGNED_INT, 0);
}
