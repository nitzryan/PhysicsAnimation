#include "Cloth.h"
#include "./rendering/BufferWriter.h"

Cloth::Cloth(std::vector<Rope> ropes) : color(ColorRGBA(.5,.1,.2,1)) {
	this->ropes = ropes;
	if (ropes.size() > 1) {
		this->side_link_len = ropes[0].get_base_pos().Subtract(ropes[1].get_base_pos()).GetMagnitude();
	}
	else {
		this->side_link_len = 0;
	}
	this->height = ropes[0].get_length();
	this->width = ropes.size();
	for (int i = 0; i < ropes.size(); i++) {
		points.push_back(ropes[i].get_base_pos());
	}
	
	for (int i = 1; i < ropes[0].get_length(); i++) {
		for (int j = 0; j < ropes.size(); j++) {
			points.push_back(ropes[j].position[i]);
		}
	}
}

void Cloth::Update(float dt, Vec3F gravity, Vec3F wind, const std::vector<SphereRenderable>& spheres) {
	// skip over first nodes as they are base and do not move
	if(ropes.size() == 0) return;

	// Apply Wind first so that velocity is unmodified by gravity
	for (int i = 0; i < ropes.size() - 1; i++) {
		ropes[i].ApplyWind(dt, wind, ropes[i + 1]);
	}

	// Apply Gravity
	for (auto& i : ropes) {
		i.ApplyGravity(dt, gravity);
	}

	ropes[0].Update_pos(dt, spheres);
	ropes[0].Update_vel(dt, spheres);

	for(int i = 1; i < ropes.size(); i++) {
		ropes[i].Update_pos(dt, spheres);
		// now need to do side relaxation
		// len should be same for all ropes
		for (int j = 0; j < relax_steps; j++) {
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
		ropes[i].Update_vel(dt, spheres);
	}

	// update points
	for (int j = 0; j < ropes.size(); j++) {
		for (int i = 0; i < ropes[0].get_length(); i++) {
			points[i + j * ropes[0].get_length()] = ropes[j].position[i];
		}
	}
}

float Cloth::get_side_len() const {
	return side_link_len;
}

void Cloth::Render(std::vector<float>& vbo, size_t vboLoc, size_t pointCount, std::vector<unsigned int>& ebo, size_t eboLoc) const
{
	// Calculate normals
	std::vector<Vec3F> normals;
	normals.reserve(points.size());
	for (size_t i = 0; i < width; i++) {
		for (size_t j = 0; j < height; j++) {
			// Calculate dw, dh
			Vec3F dw(0, 0, 0), dh(0, 0, 0);
			if (i == 0) {
				dw = points[i * height + j].Subtract(points[(i + 1) * height + j]);
			}
			else if (i == (width - 1)) {
				dw = points[(i - 1) * height + j].Subtract(points[i * height + j]);
			}
			else {
				dw = points[(i - 1) * height + j].Subtract(points[(i + 1) * height + j]);
			}
			dw.Normalize();

			if (j == 0) {
				dh = points[i * height + j].Subtract(points[i * height + j + 1]);
			}
			else if (j == (height - 1)) {
				dh = points[i * height + j - 1].Subtract(points[i * height + j]);
			}
			else {
				dh = points[i * height + j - 1].Subtract(points[i * height + j + 1]);
			}
			dh.Normalize();

			Vec3F norm = Vec3F::Cross(dw, dh);
			norm.Normalize(); // Not sure that this is necessary
			normals.push_back(norm);
		}
	}

	// Add Points
	for (size_t i = 0; i < points.size(); i++) {
		float texX = (i / height) / (float)(width - 1);
		float texY = (i % height) / (float)(height - 1);
		BufferWriter::AddPoint(vbo, vboLoc, points[i], color, normals[i], texX, texY);
	}

	// Write to EBO
	for (size_t i = 0; i < width - 1; i++) {
		for (size_t j = 0; j < height - 1; j++) {
			ebo[eboLoc] = i * height + j + pointCount;
			ebo[eboLoc + 1] = (i + 1) * height + j + pointCount;
			ebo[eboLoc + 2] = i * height + j + 1 + pointCount;
			ebo[eboLoc + 3] = (i + 1) * height + j + pointCount;
			ebo[eboLoc + 4] = (i + 1) * height + j + 1 + pointCount;
			ebo[eboLoc + 5] = i * height + j + 1 + pointCount;
			eboLoc += 6;
		}
	}
}

Material Cloth::GetMaterial() const
{
	Material mat;
	mat.color = color;
	mat.ka = 0.6;
	mat.kd = 0.6;
	mat.ks = 0.1;
	mat.specPower = 10;
	mat.textureID = 1;
	return mat;
}
