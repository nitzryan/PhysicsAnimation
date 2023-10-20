#pragma once
#include "../rendering/IRenderable.h"
#include "../geometry/Pos3F.h"
#include <vector>

#define GRAVITY 10

class ShallowWater :
    public IRenderable
{
public:
	ShallowWater(const Pos3F& pLower, const Pos3F& pUpper, int xBins, int zBins, float yRenderHeight);
	void Update(float dt);
	int NumIndices() const override;
	int NumPoints() const override;
	void Render(std::vector<float>& vbo, size_t vboLoc, size_t pointCount, std::vector<unsigned int>& ebo, size_t eboLoc) const override;
private:
	Pos3F base;
	float dx, dz;
	int xBins, zBins;
	float yRenderHeight;
	float time = 0;
	
	struct WaterBin {
		float h;
		float hx;
		float hz;
		float oldH;
		float oldHx;
		float oldHz;
		float dh;
		float dhx;
		float dhz;

		WaterBin(float h, float hx, float hz) : h(h), hx(hx), hz(hz), dh(0), dhx(0), dhz(0) {}
		float GetXTerm() const {
			return hx * hx / h + 0.5 * GRAVITY * h * h;
		}
		float GetZTerm() const {
			return hz * hz / h + 0.5 * GRAVITY * h * h;
		}
		float GetXZTerm() const {
			return hx * hz / h;
		}
	};
	
	std::vector<WaterBin> water; // Ordered in rows seperated by dx

	void UpdateDerivatives();
};

