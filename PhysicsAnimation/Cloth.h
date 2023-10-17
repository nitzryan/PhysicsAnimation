#pragma once
#include "Rope.h"

class Cloth {
public:
	Cloth(std::vector<Rope> ropes, float side_link_len);
	void Update(float dt, Vec3F gravity);
	int relax_steps = 1;
	float get_side_len();
private:
	std::vector<Rope> ropes;
	float side_link_len;
};