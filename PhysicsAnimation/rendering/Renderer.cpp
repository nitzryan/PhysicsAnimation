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
"in vec3 normal;"
"out vec4 Color;"
"uniform mat4 view;"
"uniform mat4 proj;"
"const vec3 inlightDir = normalize(vec3(0,0,-1));"
"out vec3 normalOut;"
"out vec3 pos;"
"out vec3 eyePos;"
"out vec3 lightDir;"
"void main() {"
" Color = inColor;"
" gl_Position = view * vec4(position, 1.0);"
" pos = gl_Position.xyz / gl_Position.w;"
" vec4 norm4 = transpose(inverse(view)) * vec4(normal, 0.0);"
" normalOut = norm4.xyz;"
" lightDir = (view * vec4(inlightDir, 0)).xyz;"
" gl_Position = proj * gl_Position;"
"}";

const GLchar* fragmentSource =
"#version 150 core\n"
"in vec4 Color;"
"in vec3 normalOut;"
"in vec3 pos;"
"in vec3 eyePos;"
"in vec3 lightDir;"
"out vec4 outColor;"
"const float ka = 0.6;"
"const float kd = 0.2;"
"const float ks = 0.2;"
"void main() {"
" vec3 N = normalize(normalOut);" //Re-normalized the interpolated normals
" vec3 diffuseC = kd * Color.xyz*max(dot(lightDir,N),0.0);"
" vec3 ambC = Color.xyz*ka;"
" vec3 reflectDir = reflect(-lightDir,N);"
" reflectDir = normalize(reflectDir);"
" vec3 viewDir = normalize(-pos);" //We know the eye is at 0,0
" float spec = max(dot(reflectDir,viewDir),0.0);"
" if (dot(lightDir,N) <= 0.0) spec = 0;"
" vec3 specC = vec3(ks,ks,ks)*pow(spec,10);"
" outColor = vec4(ambC+diffuseC+specC, Color.a);"
"}";


Renderer::Renderer() : cameraPos(0, 0, 0), cameraScale(1,1,1), cameraDir(0,0,-1), upDir(0,1,0)
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

	normAttrib = glGetAttribLocation(shaderProgram, "normal");
	glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE,
		vertexSize * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(normAttrib);

	glBindVertexArray(0); //Unbind the VAO so we don’t accidentally modify it

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


Renderer::~Renderer()
{
	
}

void Renderer::StartFrame()
{
	currentVboLoc = 0;
	currentIndicesLoc = 0;
}

void Renderer::Render(const IRenderable& obj)
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

	obj.Render(vboContents, currentVboLoc, currentVboLoc / vertexSize, indices, currentIndicesLoc);
	currentVboLoc += numVertices;
	currentIndicesLoc += numIndices;
}

void Renderer::SetCamera(const Camera& camera)
{
	cameraPos = camera.GetPos();
	cameraScale = camera.GetScale();
	cameraDir = camera.GetLookDir();
	upDir = camera.GetUpDir();
}

void Renderer::FinalizeFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
		glm::vec3(upDir.x, upDir.y, upDir.z)
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
