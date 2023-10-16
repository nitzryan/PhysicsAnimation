#pragma once

#include "../glad/glad.h"
#include "../geometry/Pos3F.h"
#include "Renderable.h"
#include <vector>

class Renderer
{
public:
	Renderer();
	~Renderer();
	void StartFrame();

	void SetCamera(const Pos3F& pos, const Pos3F& scale, const Vec3F& dir);
	void Render(const Renderable& obj);
	void FinalizeFrame();
private:
	GLuint vbo;
	GLuint vao;
	GLuint ebo;
	GLuint shaderProgram;
	const int vertexSize = 10;
	const int vboSize = 100000 * vertexSize;
	const int indicesSize = 100000;
	std::vector<float> vboContents;
	std::vector<unsigned int> indices;
	size_t currentVboLoc;
	size_t currentIndicesLoc;

	Pos3F cameraPos, cameraScale;
	Vec3F cameraDir;
	GLint posAttrib, colAttrib, normAttrib;
	GLuint vertexShader, fragmentShader;
};

