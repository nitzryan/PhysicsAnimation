#include "Rope.h"

Rope::Rope(int length, float link_len, Pos3F start, Vec3F dir){
	this->length = length;
	this->link_len = link_len;
	Pos3F point = start;
	for(int i =0; i < length; i++){
		position.push_back(point);
		last_position.push_back(point);
		velocity.push_back(Vec3F(0,0,0));
		start.Add(Vec3F::Mul(dir, link_len));
	}
}

void Rope::Update(float dt, Vec3F gravity) {
	for(int i = 0; i < length; i++){
		last_position[i] = position[i];
		velocity[i].Add(Vec3F::Mul(gravity, dt));
		position[i].Add(Vec3F::Mul(velocity[i], dt));
	}

	// relaxation steps
	for (int i = 0; i < relax_setps; i++) {
		for (int j = 0; j < length - 1; j++) {
			
		}
	}

}
