#pragma once
#include "Rope.h"
#include "rendering/IRenderable.h"

class Scene {
public:
	Scene();
	~Scene();
	Rope ropes[];
	bool update(float dt);
	std::vector<IRenderable*>GetRender();
private:

};

