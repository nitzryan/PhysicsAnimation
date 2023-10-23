#pragma once
#include "Cloth.h"
#include "Rope.h"
#include "rendering/Camera.h"
#include "SphereRenderable.h"
#include "rendering/IRenderable.h"
#include "rendering/Renderer.h"
#include "water/ShallowWater.h"
#include "rendering/Camera.h"
#include "rendering/PlaneRenderable.h"

class Scene {
public:
	Scene();
	Scene(int sub_steps, int relax_steps, Vec3F gravity, Vec3F wind, Renderer *renderer);
	void update(float dt);
	void add_cloth(Cloth cloth);
	void add_sphere(SphereRenderable sphere);
	void set_sub_steps(int sub_steps);
	void set_relax_steps(int relax_steps);
	void render() const;
	void SetWind(const Vec3F& w) { wind = w; }
	void LoadScene(int scene, Camera& camera);
private:
	int sub_steps;
	int relax_steps;
	Renderer *renderer;
	Vec3F gravity;
	Vec3F wind;
	std::vector<Cloth> cloths;
	std::vector<SphereRenderable> spheres;
	std::vector<PlaneRenderable> planes;
	std::vector<ShallowWater> water;
};

