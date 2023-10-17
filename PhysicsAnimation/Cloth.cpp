#include "Cloth.h"

Cloth::Cloth(std::vector<Rope> ropes, float side_link_len) {
	this->ropes = ropes;
	this->side_link_len = side_link_len;
}

void Cloth::Update(float dt, Vec3F gravity) {
	// skip over first nodes as they are base and do not move
	if(ropes.size() == 0) return;

	ropes[0].Update_pos(dt, gravity);
	ropes[1].Update_vel(dt);
	for(int i = 1; i < ropes.size(); i++) {
		ropes[i].Update_pos(dt, gravity);
		// now need to do side relaxation
		// len should be same for all ropes
		for (int j = 0; j > relax_steps; j++) {
			for (int k = 1; k < ropes[i].get_length(); k++) {
				Vec3F delta = ropes[i].position[k].Subtract(ropes[i - 1].position[k]);
				float delta_length = delta.GetMagnitude();
				float correction = delta_length - side_link_len;
				Vec3F delta_norm = delta.GetNormalized();
				ropes[i].position[k].Sub(Vec3F::Mul(delta_norm, (correction/2)));
				ropes[i - 1].position[k].Add(Vec3F::Mul(delta_norm, (correction/2)));
			}
			ropes[i].position[0] = ropes[i].get_base_pos();
			ropes[i - 1].position[0] = ropes[i - 1].get_base_pos();
		}
	}

	for(int i = 0; i < ropes.size(); i++) {
		ropes[i].Update_vel(dt);
	}
}

float Cloth::get_side_len() {
	return side_link_len;
}