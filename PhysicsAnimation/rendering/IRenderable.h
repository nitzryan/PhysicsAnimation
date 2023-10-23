#pragma once

#include <vector>
#include "Material.h"

class IRenderable {
public:
	virtual int NumIndices() const = 0;
	virtual int NumPoints() const = 0;
	virtual void Render(std::vector<float>& vbo, size_t vboLoc, size_t pointCount, std::vector<unsigned int>& ebo, size_t eboLoc) const = 0;
	virtual Material GetMaterial() const = 0;
};