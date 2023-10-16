#pragma once
#include "IRenderable.h"

#include <vector>
#include "../geometry/Pos3F.h"
#include "../geometry/Vec3F.h"
#include "ColorRGBA.h"

class TestCloth :
    public IRenderable
{
public:
	TestCloth();
	void Update(float dt);
	int NumIndices() const override { return 2 * 3 * (width - 1) * (height - 1); };
	int NumPoints() const override { return width * height; };
	void Render(std::vector<float>& vbo, size_t vboLoc, size_t pointCount, std::vector<unsigned int>& ebo, size_t eboLoc) const override;
private:
	std::vector<Pos3F> points;
	const unsigned int width = 10;
	const unsigned int height = 20;
	float time;
	ColorRGBA color;
};

