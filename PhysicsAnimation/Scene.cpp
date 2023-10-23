#include "Scene.h"
#include <iostream>

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
	if (dt > 0.05)
		dt = 0.05;

	for (int i = 0; i < sub_steps; i++) {
		for (int j = 0; j < cloths.size(); j++) {
			cloths[j].Update(dt/sub_steps, gravity, wind, spheres);
		}
		for (auto& i : water) {
			i.Update(dt);
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

void Scene::render() const {
	if (renderer != NULL) {
		for (int i = 0; i < cloths.size(); i++) {
			renderer->Render(cloths[i]);
		}
		for (auto& i : spheres) {
			renderer->Render(i);
		}
		for (auto& i : planes) {
			renderer->Render(i);
		}
		for (auto& i : water) {
			renderer->Render(i);
		}
	}
}

void Scene::LoadScene(int scene, Camera& camera)
{
	cloths.clear();
	spheres.clear();
	planes.clear();
	water.clear();

	std::vector<Rope> ropes;
	Material grass, sand, rock;
	float h = 2.5;
	switch (scene)
	{
	case 1: // US Flag on Globe Scene
		std::cout << "Loading Cloth Scene\n";
		ropes.reserve(80);
		for (int i = 0; i < 80; i++) {
			ropes.emplace_back(25, .02, Pos3F(i*.01,0,i*.01), Vec3F::Cross(Vec3F(.5,0,.5),Vec3F(0,-1,0)).GetNormalized());
		}
		cloths.emplace_back(ropes);
		spheres.emplace_back(Pos3F(.25, -0.4, .25), .25);

		camera = Camera(1, 1, Pos3F(-1, 1, 3), Vec3F(1, -1, -2), Vec3F(0, 2, -1));
		break;
	case 2: // Water Scene
		std::cout << "Loading Water Scene\n";
		water.emplace_back(Pos3F(0, 0, 0), Pos3F(2, 2, 2), 25, 25, 0.1);

		grass.color = ColorRGBA(0.1, 0.4, 0, 1);
		grass.textureID = -1;

		sand.color = ColorRGBA(0.85, 0.75, 0.50, 1);
		sand.textureID = -1;

		rock.color = ColorRGBA(0.9, 0.6, 0.4, 1);
		rock.textureID = -1;
		
		planes.emplace_back(Pos3F(-20, h, 0), Pos3F(20, h, 0), Pos3F(-20, h, -10), Pos3F(20, h, -10), grass);
		planes.emplace_back(Pos3F(-20, h, 2), Pos3F(0, h, 2), Pos3F(-20, h, 0), Pos3F(0, h, 0), grass);
		planes.emplace_back(Pos3F(2, h, 2), Pos3F(20, h, 2), Pos3F(2, h, 0), Pos3F(20, h, 0), grass);
		planes.emplace_back(Pos3F(-20, h, 10), Pos3F(20, h, 10), Pos3F(-20, h, 2), Pos3F(20, h, 2), grass);
		planes.emplace_back(Pos3F(0, 0, 0), Pos3F(0, 0, 2), Pos3F(2, 0, 0), Pos3F(2, 0, 2), rock);
		planes.emplace_back(Pos3F(0, 0, 0), Pos3F(2, 0, 0), Pos3F(0, h, 0), Pos3F(2, h, 0), sand);
		planes.emplace_back(Pos3F(2, 0, 0), Pos3F(2, 0, 2), Pos3F(2, h, 0), Pos3F(2, h, 2), sand);
		planes.emplace_back(Pos3F(2, 0, 2), Pos3F(0, 0, 2), Pos3F(2, h, 2), Pos3F(0, h, 2), sand);
		planes.emplace_back(Pos3F(0, 0, 2), Pos3F(0, 0, 0), Pos3F(0, h, 2), Pos3F(0, h, 0), sand);

		camera = Camera(1, 1, Pos3F(1, 7, 4), Vec3F(0, -2, -1), Vec3F(0, 1, -1));
		break;
	default:
		break;
	}
}

