#pragma once
#include "./rendering/IRenderable.h"
#include "Rope.h"
#include "SphereRenderable.h"

class Cloth :
	public IRenderable {
public:
	Cloth(std::vector<Rope> ropes);
	int NumIndices() const override { return 2 * 3 * (width - 1) * (height - 1); };
	int NumPoints() const override { return width * height; };
	void Render(std::vector<float>& vbo, size_t vboLoc, size_t pointCount, std::vector<unsigned int>& ebo, size_t eboLoc) const override;
	void Update(float dt, Vec3F gravity, Vec3F wind, const std::vector<SphereRenderable> &spheres);
	int relax_steps = 10;
	float get_side_len() const;
private:
	std::vector<Rope> ropes;
	float side_link_len;
	int width;
	int height;
	std::vector<Pos3F> points;
	ColorRGBA color;
};