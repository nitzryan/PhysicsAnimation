#include "SphereRenderable.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "rendering/BufferWriter.h"

SphereRenderable::SphereRenderable(Pos3F center, float radius) : color(ColorRGBA(.1,.5,.1,1)) {
	this->center = center;
	this->radius = radius;
	thetaSlices = 30;
	phiSlices = 30;
	GenerateRenderPoints();
}

float SphereRenderable::get_radius() {
	return radius;
}

Pos3F SphereRenderable::get_center() {
	return center;
}

void SphereRenderable::SetThetaSlices(int s)
{
	if (s > 0) {
		thetaSlices = s;
		GenerateRenderPoints();
	}
}

void SphereRenderable::SetPhiSlices(int s)
{
	if (s > 0) {
		phiSlices = s;
		GenerateRenderPoints();
	}
}

void SphereRenderable::GenerateRenderPoints()
{
	renderPoints.clear();
	normals.clear();
	int numPoints = (thetaSlices) * (phiSlices + 1);
	renderPoints.reserve(numPoints);
	normals.reserve(numPoints);

	float thetaStep = 2 * M_PI / thetaSlices;
	float phiStep = M_PI / phiSlices;
	for (size_t p = 0; p <= phiSlices; p++) {
		float phi = -M_PI / 2 + p * phiStep;
		for (size_t t = 0; t < thetaSlices; t++) {
			float theta = t * thetaStep;
			float x = center.x + radius * cosf(theta) * cosf(phi);
			float y = center.y + radius * sinf(phi);
			float z = center.z + radius * sinf(theta) * cosf(phi);
			renderPoints.emplace_back(x, y, z);
			Vec3F normal = center.Subtract(renderPoints.back());
			normals.push_back(normal);
		}
	}
}

void SphereRenderable::Render(std::vector<float>& vbo, size_t vboLoc, size_t pointCount, std::vector<unsigned int>& ebo, size_t eboLoc) const \
{
	for (size_t i = 0; i < renderPoints.size(); i++) {
		BufferWriter::AddPoint(vbo, vboLoc, renderPoints[i], color, normals[i]);
	}

	// Write to EBO
	for (size_t p = 0; p < phiSlices; p++) {
		for (size_t t = 0; t < thetaSlices; t++) {
			int tRight = (t + 1) % thetaSlices; // Need to handle wrap around
			ebo[eboLoc] = p * thetaSlices + t + pointCount;
			ebo[eboLoc + 1] = (p + 1) * thetaSlices + t + pointCount;
			ebo[eboLoc + 2] = p * thetaSlices + tRight + pointCount;
			ebo[eboLoc + 3] = p * thetaSlices + tRight + pointCount;
			ebo[eboLoc + 4] = (p + 1) * thetaSlices + t + pointCount;
			ebo[eboLoc + 5] = (p + 1) * thetaSlices + tRight + pointCount;
			eboLoc += 6;
		}
	}
}