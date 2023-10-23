#include "Rope.h"
#include <assert.h>

bool Rope::DragIsOn = true;

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

void Rope::ApplyGravity(float dt, const Vec3F& gravity)
{
	Vec3F gravStep = Vec3F::Mul(gravity, dt);
	for (size_t i = 1; i < length; i++) {
		velocity[i] += gravStep;
	}
}

void Rope::ApplyWind(float dt, const Vec3F& wind, Rope& rope)
{
	if (!DragIsOn)
		return;
	assert(length == rope.length && length > 0);

	auto GetAccel = [wind](const Pos3F& p1, const Pos3F& p2, const Pos3F& p3, const Vec3F& v1, const Vec3F& v2, const Vec3F& v3, const Vec3F& v4) {
		Vec3F v = Vec3F::Mul(v1 + v2 + v3 + v4, 0.25f);
		v.Sub(wind);
		const float p = 1.2; // Density of Air
		const float cd = 0.2; // Need to modify this to get behavior correct

		// Calculate normal
		Vec3F v12 = p1.Subtract(p2);
		Vec3F v13 = p1.Subtract(p3);
		Vec3F normal = Vec3F::Cross(v12, v13);
		float a0 = 0.5 * normal.GetMagnitude();
		normal.Normalize();

		// Calculate Area
		float a;
		float magV = v.GetMagnitude();
		if (magV != 0)
			a = a0 * Vec3F::Dot(v, normal) / v.GetMagnitude();
		else
			a = 0;

		const float density = 8;
		return Vec3F::Mul(normal, p * magV * magV * cd * a / a0 / density); // 0.5 for formula and 2 for area measured being half cancel out
	};

	float fourthDt = 0.25 * dt;
	for (size_t i = 0; i < length - 1; i++) {
		Vec3F accel = GetAccel(position[i], position[i + 1], rope.position[i], velocity[i], velocity[i + 1], rope.velocity[1], rope.velocity[i + 1]);
		accel.Mul(fourthDt);
		velocity[i] += accel;
		velocity[i + 1] += accel;
		rope.velocity[i] += accel;
		rope.velocity[i + 1] += accel;
	}

	velocity[0] = Vec3F(0,0,0);
	rope.velocity[0] = Vec3F(0,0,0);
}

void Rope::Update_pos(float dt, const std::vector<SphereRenderable>& spheres) {
	bool  * collides = new bool[length];
	for(int i = 0; i < length; i++){
		last_position[i] = position[i];

		position[i].Add(Vec3F::Mul(velocity[i], dt));
		collides[i] = false;
		for (int j = 0; j < spheres.size(); j++) {
			float radius = spheres[j].get_radius();
			Pos3F center = spheres[j].get_center();
			if (position[i].Subtract(center).GetMagnitude() < radius + 0.01) {
				Vec3F normal = position[i].Subtract(center).GetNormalized();
				Vec3F move = Vec3F::Mul(normal, radius + .01);
				position[i] = Pos3F::Add(center, move);
				velocity[i].Sub(Vec3F::Mul(normal,1.05));
				collides[i] = true;
			}
		}
	}

	// relaxation steps
	for (int i = 0; i < relax_setps; i++) {
		for (int j = 0; j < length - 1; j++) {
			Vec3F delta = position[j + 1].Subtract(position[j]);
			float delta_length = delta.GetMagnitude();
			float correction = delta_length - link_len;
			Vec3F delta_norm = delta.GetNormalized();

			/*if (collides[j] && collides[j + 1]) {
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
			}*/
			
			position[j + 1].Sub(Vec3F::Mul(delta_norm, (correction / 2)));
			position[j].Add(Vec3F::Mul(delta_norm, (correction / 2)));
		}
		// update base pos
		position[0] = base_pos;
	}
	delete[] collides;

	
}


void Rope::Update_vel(float dt, const std::vector<SphereRenderable> &spheres) {
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
