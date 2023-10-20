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

void Rope::Update_pos(float dt, Vec3F gravity, std::vector<SphereRenderable>& spheres) {

	bool  * collides = new bool[length];
	for(int i = 0; i < length; i++){
		last_position[i] = position[i];
		collides[i] = false;
		for (int j = 0; j < spheres.size(); j++) {
			float radius = spheres[j].get_radius();
			Pos3F center = spheres[j].get_center();
			if (position[i].Subtract(center).GetMagnitude() < radius) {
				Vec3F normal = position[i].Subtract(center).GetNormalized();
				Vec3F move = Vec3F::Mul(normal, radius + .01);
				position[i] = Pos3F::Add(center, move);
				velocity[i].Sub(Vec3F::Mul(normal,1.05));
				collides[i] = true;
			}
		}
		if (!collides[i]) {
			velocity[i].Add(Vec3F::Mul(gravity, dt));
			position[i].Add(Vec3F::Mul(velocity[i], dt));
		}
		
		
	}

	// relaxation steps
	for (int i = 0; i < relax_setps; i++) {
		for (int j = 0; j < length - 1; j++) {
			Vec3F delta = position[j + 1].Subtract(position[j]);
			float delta_length = delta.GetMagnitude();
			float correction = delta_length - link_len;
			Vec3F delta_norm = delta.GetNormalized();

			if (collides[j] && collides[j + 1]) {
				position[j + 1].Sub(Vec3F::Mul(delta_norm, (correction / 2)));
				position[j].Add(Vec3F::Mul(delta_norm, (correction / 2)));
			}
			else if (collides[j]) {
				position[j].Add(Vec3F::Mul(delta_norm, (correction)));
			}
			else if (collides[i]) {
				position[j + 1].Sub(Vec3F::Mul(delta_norm, (correction)));

			}
			else {	
				position[j + 1].Sub(Vec3F::Mul(delta_norm, (correction / 2)));
				position[j].Add(Vec3F::Mul(delta_norm, (correction / 2)));
			}
			
		}
		// update base pos
		position[0] = base_pos;
	}
	delete[] collides;

	
}


void Rope::Update_vel(float dt, std::vector<SphereRenderable> &spheres) {
	// update velocity
	for (int i = 0; i < length; i++) {
		bool collides = false;
		for (int j = 0; j < spheres.size(); j++) {
			float radius = spheres[j].get_radius();
			Pos3F center = spheres[j].get_center();
			if (position[i].Subtract(center).GetMagnitude() < radius) {
				Vec3F normal = position[i].Subtract(center).GetNormalized();
				position[i] = Pos3F::Add(center, Vec3F::Mul(normal, radius + .001));
				velocity[i] = Vec3F(0, 0, 0);
			}
		}
		if (!collides) {
			velocity[i] = Vec3F::Mul(position[i].Subtract(last_position[i]), (1 / dt));
		}
	}
}

int Rope::get_length() {
	return length;
}

Pos3F Rope::get_base_pos() {
	return base_pos;
}
