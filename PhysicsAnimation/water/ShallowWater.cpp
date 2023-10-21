#include "ShallowWater.h"
#include "../rendering/BufferWriter.h"
#include <iostream>

const float PI = 3.1415;

ShallowWater::ShallowWater(const Pos3F& pLower, const Pos3F& pUpper, int xBins, int zBins, float yRenderHeight) :
	xBins(xBins), zBins(zBins), yRenderHeight(yRenderHeight), base(pLower)
{
	dx = (pUpper.x - pLower.x) / (xBins - 1);
	dz = (pUpper.z - pLower.z) / (zBins - 1);

	water.reserve(xBins * zBins);
	for (size_t i = 0; i < zBins; i++) {
		for (size_t j = 0; j < xBins; j++) {
			water.emplace_back(2 + 0.4 * sinf(i * PI / zBins) + 0.4 * cosf(j * PI / xBins), 0, 0);
		}
	}

	sumHeights = 0;
	for (auto& i : water) {
		sumHeights += i.h;
	}
}

void ShallowWater::Update(float dt)
{
	time += dt;
	dt = 0.01;
	while (time >= 0.01) {
		time -= 0.01;
	
		std::vector<WaterBin> midpointsX, midpointsZ;
	
		// Create midpoints bins
		midpointsX.reserve((xBins - 1) * zBins);
		for (int i = 0; i < zBins; i++) {
			for (int j = 0; j < xBins - 1; j++) {
				int idx = i * xBins + j;
				int idxr = idx + 1;
				midpointsX.emplace_back(water[idx], water[idxr], true, dx);
			}
		}

		midpointsZ.reserve(xBins * (zBins - 1));
		for (int i = 0; i < zBins - 1; i++) {
			for (int j = 0; j < xBins; j++) {
				int idx = i * xBins + j;
				int idxd = idx + xBins;
				midpointsZ.emplace_back(water[idx], water[idxd], false, dz);
			}
		}

		// Update With half time step
		for (auto& i : midpointsX) {
			i.Step(dt / 2, 1);
		}
		for (auto& i : midpointsZ) {
			i.Step(dt / 2, 1);
		}

		// Update derivatives in non-midpoints
		for (auto& i : water) {
			i.dh = 0;
			i.dhu = 0;
			i.dhv = 0;
		}
		for (int i = 0; i < zBins; i++) {
			for (int j = 1; j < xBins - 1; j++) {
				int idx = j + i * (xBins - 1);
				int idxl = idx - 1;
				int idxw = j + i * xBins;

				water[idxw].dh -= (midpointsX[idx].hu - midpointsX[idxl].hu) / dx;
				water[idxw].dhu -= (midpointsX[idx].GetXTerm() - midpointsX[idxl].GetXTerm()) / dx;
				water[idxw].dhv -= (midpointsX[idx].GetXZTerm() - midpointsX[idxl].GetXZTerm()) / dx;
			}
		}
		for (int i = 1; i < zBins - 1; i++) {
			for (int j = 0; j < xBins; j++) {
				int idx = j + i * xBins;
				int idxu = idx - xBins;
				int idxw = j + i * xBins;

				water[idxw].dh -= (midpointsZ[idx].hv - midpointsZ[idxu].hv) / dz;
				water[idxw].dhu -= (midpointsZ[idx].GetXZTerm() - midpointsZ[idxu].GetXZTerm()) / dz;
				water[idxw].dhv -= (midpointsZ[idx].GetZTerm() - midpointsZ[idxu].GetZTerm()) / dz;
			}
		}

		// Update All points not along edge
		for (size_t i = 1; i < zBins - 1; i++) {
			for (size_t j = 1; j < xBins - 1; j++) {
				water[i * xBins + j].Step(dt);
			}
		}
		// For edges, set reflective on edge, that calculate non-edge step
		// Top
		for (size_t j = 1; j < xBins - 1; j++) {
			water[j].h = (water[j + xBins].h + water[j].h) / 2;
			water[j].hu = -water[j + xBins].hu;
			water[j].hv = -water[j + xBins].hv;
			water[j].Step(dt);
		}
		// Bot
		for (size_t j = 1; j < xBins - 1; j++) {
			size_t idx = (zBins - 1) * xBins + j;
			water[idx].h = (water[idx - xBins].h + water[idx].h) / 2;
			water[idx].hu = -water[idx - xBins].hu;
			water[idx].hv = -water[idx - xBins].hv;
			water[idx].Step(dt);
		}
		// Left
		for (size_t i = 1; i < zBins - 1; i++) {
			size_t idx = xBins * i;
			water[idx].h = (water[idx + 1].h + water[idx].h) / 2;
			water[idx].hu = -water[idx + 1].hu;
			water[idx].hv = -water[idx + 1].hv;
			water[idx].Step(dt);
		}
		// Right
		for (size_t i = 1; i < zBins - 1; i++) {
			size_t idx = xBins * (i + 1) - 1;
			water[idx].h = (water[idx - 1].h + water[idx].h) / 2;
			water[idx].hu = -water[idx - 1].hu;
			water[idx].hv = -water[idx - 1].hv;
			water[idx].Step(dt);
		}

		// Set corners reflectively
		water[0] = WaterBin(water[1], water[xBins]);
		water[xBins - 1] = WaterBin(water[xBins - 2], water[2 * xBins - 1]);
		water[water.size() - xBins] = WaterBin(water[water.size() - 2 * xBins], water[water.size() - xBins + 1]);
		water[water.size() - 1] = WaterBin(water[water.size() - 2], water[water.size() - xBins - 1]);
	
		// Trend all heights towards the steady state height
		float aveHeight = sumHeights / (water.size());
		const float heightFactor = 0.005;
		for (auto& i : water) {
			i.h += dt * (aveHeight - i.h) * heightFactor;
		}
	}
}

int ShallowWater::NumIndices() const
{
	return //6 // Bottom
		//+ 6 * 2 * (xBins + zBins) +// Edges
		6 * (xBins) * (zBins); 
}

int ShallowWater::NumPoints() const
{
	return //4 * (xBins + zBins) // Edges top and bottom
		0 + (xBins + 1) * (zBins + 1); // Top that is not edges
}

void ShallowWater::Render(std::vector<float>& vbo, size_t vboLoc, size_t pointCount, std::vector<unsigned int>& ebo, size_t eboLoc) const
{
	// Convert bins into points
	std::vector<Pos3F> points;
	int xWidth = xBins + 1;
	int zWidth = zBins + 1;
	points.reserve(xWidth * zWidth);
	
	// First X-Row
	size_t idx = 0;
	points.emplace_back(base.x, water[idx].h, base.z);
	for (size_t i = 0; i < xBins - 1; i++) {
		points.emplace_back(base.x + dx * (i + 1), (water[i].h + water[i + 1].h) / 2, base.z);
	}
	points.emplace_back(base.x + xBins * dx, water[idx + xBins - 1].h, base.z);
	// Middle xBins - 2 X-Rows
	for (size_t i = 1; i < zBins; i++) {
		idx = xBins * i;
		points.emplace_back(base.x, (water[idx].h + water[idx - xBins].h) / 2, base.z + i * dz);
		for (size_t j = 0; j < xBins - 1; j++) {
			size_t ll = i * xBins + j;
			size_t ul = ll - xBins;
			size_t ur = ul + 1;
			size_t lr = ll + 1;
			points.emplace_back(base.x + dx * (j+1), (water[ul].h + water[ur].h + water[ll].h + water[lr].h)/4, base.z + i * dz);
		}
		points.emplace_back(base.x + xBins * dx, (water[idx + xBins - 1].h + water[idx - 1].h) / 2, base.z + i * dz);
	}
	// Last X-Row
	idx = (zBins - 1) * xBins;
	points.emplace_back(base.x, water[idx].h, base.z + zBins * dz);
	for (size_t i = 0; i < xBins - 1; i++) {
		points.emplace_back(base.x + dx * (i + 1), (water[idx + i].h + water[idx + i + 1].h) / 2, base.z + zBins * dz);
	}
	points.emplace_back(base.x + xBins * dx, water[idx + xBins - 1].h, base.z + zBins * dz);

	// Get Normals
	std::vector<Vec3F> normals;
	normals.reserve(xWidth * zWidth);
	for (size_t j = 0; j < zWidth; j++) {
		for (size_t i = 0; i < xWidth; i++) {
			size_t idx = xWidth * j + i;
			
			Vec3F deltaX;
			if (i == 0) {
				deltaX = points[idx] - points[idx + 1];
			}
			else if (i == (xWidth - 1)) {
				deltaX = points[idx - 1] - points[idx];
			}
			else {
				deltaX = points[idx - 1] - points[idx + 1];
			}
			
			Vec3F deltaZ;
			if (j == 0) {
				deltaZ = points[idx] - points[idx + xWidth];
			}
			else if (j == (zWidth - 1)) {
				deltaZ = points[idx - xWidth] - points[idx];
			}
			else {
				deltaZ = points[idx - xWidth] - points[idx + xWidth];
			}

			Vec3F normal = Vec3F::Cross(deltaZ, deltaX);
			normal.Normalize();
			normals.push_back(normal);
		}
	}

	ColorRGBA color = ColorRGBA(0, 0.1, 0.9, 0.7);
	// Add Top Points
	for (size_t i = 0; i < points.size(); i++) {
		BufferWriter::AddPoint(vbo, vboLoc, points[i], color, normals[i]);
	}

	// Render top
	for (size_t i = 0; i < zWidth - 1; i++) {
		for (size_t j = 0; j < xWidth - 1; j++) {
			ebo[eboLoc] = i * xWidth + j + pointCount;
			ebo[eboLoc + 1] = (i + 1) * xWidth + j + pointCount;
			ebo[eboLoc + 2] = i * xWidth + j + 1 + pointCount;
			ebo[eboLoc + 3] = (i + 1) * xWidth + j + pointCount;
			ebo[eboLoc + 4] = (i + 1) * xWidth + j + 1 + pointCount;
			ebo[eboLoc + 5] = i * xWidth + j + 1 + pointCount;
			eboLoc += 6;
		}
	}
}
