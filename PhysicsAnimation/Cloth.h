#pragma once
#include "./rendering/IRenderable.h"
#include "Rope.h"

class Cloth :
	public IRenderable {
public:
	Cloth(std::vector<Rope> ropes, float side_link_len);
	int NumIndices() const override { return 2 * 3 * (width - 1) * (height - 1); };
	int NumPoints() const override { return width * height; };
	void Render(std::vector<float>& vbo, size_t vboLoc, size_t pointCount, std::vector<unsigned int>& ebo, size_t eboLoc) const override;
	void Update(float dt, Vec3F gravity);
	int relax_steps = 1;
	float get_side_len();
private:
	std::vector<Rope> ropes;
	float side_link_len;
	int width;
	int height;
	std::vector<Pos3F> points;
	ColorRGBA color;
};