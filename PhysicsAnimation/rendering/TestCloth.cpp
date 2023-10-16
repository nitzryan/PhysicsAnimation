#include "TestCloth.h"

#include "BufferWriter.h"
#include <cmath>

TestCloth::TestCloth() : color(ColorRGBA(0.4, 0.1, 0.2, 1))
{
	time = 0;

	for (size_t i = 0; i < width; i++) {
		for (size_t j = 0; j < height; j++) {
			float x = -0.5 + 0.05 * j;
			float y = -0.25 + 0.05 * i;
			points.emplace_back(x, y, 0);
		}
	}
}

void TestCloth::Update(float dt)
{
	time += dt;
	for (size_t i = 0; i < width; i++) {
		for (size_t j = 0; j < height; j++) {
			float offset = (i + j) * 0.1;
			points[i * height + j].z = 0.4 * cos(time + offset);
		}
	}
}

void TestCloth::Render(std::vector<float>& vbo, size_t vboLoc, size_t pointCount, std::vector<unsigned int>& ebo, size_t eboLoc) const
{
	// Calculate normals
	std::vector<Vec3F> normals;
	normals.reserve(points.size());
	for (size_t i = 0; i < width; i++) {
		for (size_t j = 0; j < height; j++) {
			// Calculate dw, dh
			Vec3F dw(0,0,0), dh(0,0,0);
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
		BufferWriter::AddPoint(vbo, vboLoc, points[i], color, normals[i]);
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
