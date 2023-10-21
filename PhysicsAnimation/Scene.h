#pragma once
#include "Cloth.h"
#include "Rope.h"
#include "SphereRenderable.h"
#include "rendering/IRenderable.h"
#include "rendering/Renderer.h"


class Scene {
public:
	Scene();
	Scene(int sub_steps, int relax_steps, Vec3F gravity, Vec3F wind, Renderer *renderer);
	void update(float dt);
	void add_cloth(Cloth cloth);
	void add_sphere(SphereRenderable sphere);
	void set_sub_steps(int sub_steps);
	void set_relax_steps(int relax_steps);
	void render();
private:
	int sub_steps;
	int relax_steps;
	Renderer *renderer;
	Vec3F gravity;
	Vec3F wind;
	std::vector<Cloth> cloths;
	std::vector<SphereRenderable> spheres;

};

