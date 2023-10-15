#pragma once

#include <vector>

class Renderable {
public:
	virtual int NumIndices() const = 0;
	virtual int NumPoints() const = 0;
	virtual void Render(std::vector<float>& vbo, size_t vboLoc, std::vector<unsigned int>& ebo, size_t eboLoc) const = 0;
};