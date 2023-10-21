#include "Scene.h"

Scene::Scene() : gravity(Vec3F(0,-10,0)), wind(Vec3F(1,0,0)){
	this->sub_steps = 1;
	this->relax_steps = 1;
	this->renderer = NULL;
}

Scene::Scene(int sub_steps, int relax_steps, Vec3F gravity, Vec3F wind, Renderer *renderer) : gravity(gravity), wind(wind){
	this->sub_steps = sub_steps;
	this->relax_steps = relax_steps;
	this->renderer = renderer;
}

void Scene::update(float dt) {
	for (int i = 0; i < sub_steps; i++) {
		for (int j = 0; j < cloths.size(); j++) {
			cloths[j].Update(dt/sub_steps, gravity, wind, spheres);
		}
	}
}

void Scene::add_cloth(Cloth cloth) {
	cloths.push_back(cloth);
}

void Scene::add_sphere(SphereRenderable sphere) {
	spheres.push_back(sphere);
}

void Scene::set_sub_steps(int sub_steps) {
	this->sub_steps = sub_steps;
}

void Scene::set_relax_steps(int relax_steps) {
	this->relax_steps = relax_steps;
}

void Scene::render() {
	if (renderer != NULL) {
		for (int i = 0; i < cloths.size(); i++) {
			renderer->Render(cloths[i]);
		}
		for (auto& i : spheres) {
			renderer->Render(i);
		}
	}
}
