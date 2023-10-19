#include "Sphere.h"

Sphere::Sphere(Pos3F center, float radius) : color(ColorRGBA(.1,.5,.1,1)) {
	this->center = center;
	this->radius = radius;
}

float Sphere::get_radius() {
	return radius;
}

Pos3F Sphere::get_center() {
	return center;
}

void Sphere::Render(std::vector<float>& vbo, size_t vboLoc, size_t pointCount, std::vector<unsigned int>& ebo, size_t eboLoc) const {
	return;
}