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
			water.emplace_back(2 + 0.2 * sinf(i * PI / zBins) + 0.2 * cosf(j * PI / xBins), 0, 0);
		}
	}
}

void ShallowWater::Update(float dt)
{
	time += dt;
	if (time < 0.01) {
		return;
	}
	time -= 0.01;
	dt = 0.01;

	// Calculate midpoints and momentums
	std::vector<float> hMidX, hMidZ, dhMidDt, dhxdt, dhzdt, hxMidX, hzMidX, hxMidZ, hzMidZ;
	size_t midSize = (xBins - 1) * (zBins - 1);
	hMidX.reserve(midSize);
	hMidZ.reserve(midSize);
	dhMidDt.reserve(midSize);
	dhxdt.reserve(midSize);
	dhzdt.reserve(midSize);
	hxMidX.reserve(midSize);
	hzMidX.reserve(midSize);
	hxMidZ.reserve(midSize);
	hzMidZ.reserve(midSize);

	// Get Midpoints
	for (size_t i = 0; i < zBins - 1; i++) {
		for (size_t j = 0; j < xBins - 1; j++) {
			size_t idx = i * xBins + j;
			size_t idxRight = idx + 1;
			size_t idxDown = idx + xBins;
			hMidX.push_back((water[idx].h + water[idxRight].h) / 2);
			hMidZ.push_back((water[idx].h + water[idxDown].h) / 2);
			hxMidX.push_back((water[idx].hx + water[idxRight].hx) / 2);
			hzMidX.push_back((water[idx].hz + water[idxRight].hz) / 2);
			hxMidZ.push_back((water[idx].hx + water[idxDown].hx) / 2);
			hzMidZ.push_back((water[idx].hz + water[idxDown].hz) / 2);

			float dhudx_mid = (water[idx].h - water[idxRight].h) / dx;
			float dhvdz_mid = (water[idx].h - water[idxDown].h) / dz;
			dhMidDt.push_back(-dhudx_mid - dhvdz_mid);

			// Compute dhxdt
			float hx, hxRight, hxDown, hz, hzRight, hzDown;
			hx = water[idx].hx;
			hz = water[idx].hz;
			hxRight = water[idxRight].hx;
			hzRight = water[idxRight].hz;
			hxDown = water[idxDown].hx;
			hzDown = water[idxDown].hz;
			float h, hRight, hDown;
			h = water[idx].h;
			hRight = water[idxRight].h;
			hDown = water[idxDown].h;
			float dhu2dx_mid = 0.5 * (hxRight * hxRight / hRight - hx * hx / h) / dx;
			float dgh2dx_mid = GRAVITY * (hRight * hRight - h * h) / dx;
			float dhuvdz_mid = (hxDown * hzDown / hDown - hx * hz / h) / dz;
			dhxdt.push_back(-dhu2dx_mid - dgh2dx_mid - dhuvdz_mid);
			float dhv2dz_mid = 0.5 * (hzDown * hzDown / hDown - hz * hz / h) / dz;
			float dgh2dz_mid = GRAVITY * (hDown * hDown - h * h) / dz;
			float dhuvdx_mid = (hxRight * hzRight / hRight - hx * hz / h) / dx;
			dhzdt.push_back(-dhv2dz_mid - dgh2dz_mid - dhuvdx_mid);
		}
	}

	// Update for 0.5 timesteps
	for (size_t i = 0; i < hMidX.size(); i++) {
		hMidX[i] += dhMidDt[i] * 0.5 * dt;
		hxMidX[i] += dh
	}

	//// Calculate Midpoint
	//for (auto& i : water) {
	//	i.oldH = i.h;
	//	i.oldHx = i.hx;
	//	i.oldHz = i.hz;
	//}

	//UpdateDerivatives();

	//for (auto& i : water) {
	//	i.h += 0.5 * dt * i.dh;
	//	i.hx += 0.5 * dt * i.dhx;
	//	i.hz += 0.5 * dt * i.dhz;
	//}

	//UpdateDerivatives();

	//// Apply midpoint derivitives to old position
	//const float dampen = 0.8;
	//for (auto& i : water) {
	//	i.dh *= dampen;
	//	i.dhx *= dampen;
	//	i.dhz *= dampen;

	//	i.h = i.oldH + dt * i.dh;
	//	i.hx = i.oldHx + dt * i.dhx;
	//	i.hz = i.oldHz + dt * i.dhz;
	//}

	//// Output energy
	//float energy = 0;
	//for (auto& i : water) {
	//	energy += i.h * i.h;
	//}
	//std::cout << "Total Energy: " << energy << std::endl;
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

void ShallowWater::UpdateDerivatives()
{
	// Reset derivatives to 0
	for (auto& i : water) {
		i.dh = 0;
		i.dhx = 0;
		i.dhz = 0;
	}

	// Update dhx, dhz
	for (size_t i = 0; i < zBins; i++) {
		for (size_t j = 0; j < xBins; j++) {
			size_t idx = i * xBins + j;
			size_t idxRight = idx + 1;
			size_t idxBot = idx + xBins;
			// Left, Top will be set from the opposite of Right, Bot for another cell
			// Flow from Right
			if (j != xBins - 1) {
				float dhx = water[idxRight].GetXTerm() - water[idx].GetXTerm();
				float dhz = water[idxRight].GetXZTerm() - water[idx].GetXZTerm();
				dhx /= dx;
				dhz /= dz;

				water[idxRight].dhx += dhx;
				water[idx].dhx -= dhx;
				water[idxRight].dhz += dhz;
				water[idx].dhz -= dhz;
			}
			// Flow from Bot
			if (i != zBins - 1) {
				float dhx = water[idxBot].GetXZTerm() - water[idx].GetXZTerm();
				float dhz = water[idxBot].GetZTerm() - water[idx].GetZTerm();

				water[idxBot].dhx += dhx;
				water[idx].dhx -= dhx;
				water[idxBot].dhz += dhz;
				water[idx].dhz -= dhz;
			}
		}
	}

	// Update dh
	for (size_t i = 0; i < zBins; i++) {
		for (size_t j = 0; j < xBins; j++) {
			size_t idx = i * xBins + j;
			size_t idxRight = idx + 1;
			size_t idxBot = idx + xBins;
			// Left, Top will be set from the opposite of Right, Bot for another cell
			// Flow from Right
			if (j != xBins - 1) {
				float vx = water[idxRight].hx - water[idx].hx;
				water[idxRight].dh -= vx;
				water[idx].dh += vx;
			}
			// Flow from Bot
			if (i != zBins - 1) {
				float vz = water[idxBot].hz - water[idx].hz;
				water[idxBot].dh -= vz;
				water[idx].dh += vz;
			}
		}
	}
}
