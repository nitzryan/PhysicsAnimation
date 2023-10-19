#include "Scene.h"

Scene::Scene() : gravity(Vec3F(0,-10,0)){
	this->sub_steps = 1;
	this->relax_steps = 1;
	this->renderer = NULL;
}

Scene::Scene(int sub_steps, int relax_steps, Vec3F gravity, Renderer *renderer) : gravity(gravity){
	this->sub_steps = sub_steps;
	this->relax_steps = relax_steps;
	this->renderer = renderer;
}

void Scene::update(float dt) {
	for (int i = 0; i < sub_steps; i++) {
		for (int j = 0; j < cloths.size(); j++) {
			cloths[j].Update(dt/sub_steps, gravity, spheres);
		}
		for (int j = 0; j < ropes.size(); j++) {
			ropes[j].Update_pos(dt/sub_steps, gravity);
			ropes[j].Update_vel(dt/sub_steps,spheres);
		}
	}
}

void Scene::add_cloth(Cloth cloth) {
	cloths.push_back(cloth);
}

void Scene::add_rope(Rope rope) {
	ropes.push_back(rope);
}

void Scene::add_sphere(Sphere sphere) {
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
		
	}
}
