#pragma once
#include <vector>
#include "./geometry/Pos3F.h"
#include "./geometry/Vec3F.h"
#include "rendering/ColorRGBA.h"
#include "./rendering/IRenderable.h"

class Sphere :
	public IRenderable{
public:
	Sphere(Pos3F center, float radius);
	int NumIndices() const override { return 0; };
	int NumPoints() const override { return 1; };
	void Render(std::vector<float>& vbo, size_t vboLoc, size_t pointCount, std::vector<unsigned int>& ebo, size_t eboLoc) const override;
	//void Update(float dt); // porbbaly not needed as it will not move
	float get_radius();
	Pos3F get_center();
private:
	Pos3F center;
	float radius;
	ColorRGBA color;
};