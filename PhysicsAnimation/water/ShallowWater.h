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
		float hu;
		float hv;
		float dh;
		float dhu;
		float dhv;

		WaterBin(float h, float hu, float hv) : h(h), hu(hu), hv(hv), dh(0), dhu(0), dhv(0) {}
		WaterBin(const WaterBin& b1, const WaterBin& b2, bool dirX, float binStep) {
			h = (b1.h + b2.h) / 2;
			hu = (b1.hu + b2.hu) / 2;
			hv = (b1.hv + b2.hv) / 2;
			if (dirX) {
				dh = -(b2.hu - b1.hu) / binStep;
				dhu = -(b2.GetXTerm() - b1.GetXTerm()) / binStep;
				dhv = -(b2.GetXZTerm() - b1.GetXZTerm()) / binStep;
			}
			else {
				dh = -(b2.hv - b1.hv) / binStep;
				dhu = -(b2.GetXZTerm() - b1.GetXZTerm()) / binStep;
				dhv = -(b2.GetZTerm() - b1.GetZTerm()) / binStep;
			}
		}

		float GetXTerm() const {
			return hu * hu / h + 0.5 * GRAVITY * h * h;
		}
		float GetZTerm() const {
			return hv * hv / h + 0.5 * GRAVITY * h * h;
		}
		float GetXZTerm() const {
			return hu * hv / h;
		}
		void Step(float dt, float dampen = 0.6) {
			dt *= dampen;
			h += dh * dt;
			hu += dhu * dt;
			hv += dhv * dt;
		}
	};
	
	std::vector<WaterBin> water; // Ordered in rows seperated by dx
};

