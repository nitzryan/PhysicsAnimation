#include "Rope.h"

Rope::Rope(int length, float link_len, Pos3F start, Vec3F dir) {
	this->length = length;
	this->link_len = link_len;
	this->base_pos = start;
	Pos3F point = start;
	for(int i =0; i < length; i++){
		position.push_back(start);
		last_position.push_back(start);
		velocity.push_back(Vec3F(0,0,0));
		start.Add(Vec3F::Mul(dir, link_len));
	}
}

void Rope::Update_pos(float dt, Vec3F gravity) {
	for(int i = 0; i < length; i++){
		last_position[i] = position[i];
		velocity[i].Add(Vec3F::Mul(gravity, dt));
		position[i].Add(Vec3F::Mul(velocity[i], dt));
	}

	// relaxation steps
	for (int i = 0; i < relax_setps; i++) {
		for (int j = 0; j < length - 1; j++) {
			Vec3F delta = position[j+1].Subtract(position[j]);
			float delta_length = delta.GetMagnitude();
			float correction = delta_length - link_len;
			Vec3F delta_norm = delta.GetNormalized();
			position[j + 1].Sub(Vec3F::Mul(delta_norm, (correction/2)));
			position[j].Add(Vec3F::Mul(delta_norm, (correction/2)));
		}
		// update base pos
		position[0] = base_pos;
	}

	
}


void Rope::Update_vel(float dt) {
	// update velocity
	for (int i = 0; i < length; i++) {
		velocity[i] = Vec3F::Mul(position[i].Subtract(last_position[i]), (1 / dt));
	}
}

int Rope::get_length() {
	return length;
}

Pos3F Rope::get_base_pos() {
	return base_pos;
}
