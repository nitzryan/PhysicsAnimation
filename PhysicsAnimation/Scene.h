#pragma once
#include "Cloth.h"
#include "Rope.h"
#include "Sphere.h"
#include "rendering/IRenderable.h"


class Scene {
public:
	Scene();
	Scene(int sub_steps, int relax_steps, Vec3F gravity);
	void update(float dt);
	void add_cloth(Cloth cloth);
	void add_rope(Rope rope);
	void add_sphere(Sphere sphere);
	void set_sub_steps(int sub_steps);
	void set_relax_steps(int relax_steps);
private:
	int sub_steps;
	int relax_steps;
	Vec3F gravity;
	std::vector<Rope> ropes;
	std::vector<Cloth> cloths;
	std::vector<Sphere> spheres;

};

