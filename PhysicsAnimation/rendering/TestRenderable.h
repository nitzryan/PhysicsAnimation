#pragma once

// This will get deleted in a future version, keeping as a reference for now.
// Render method is incorrect, should use the BufferWriter Class so that if buffer data needs to change, compilation fails

#include "IRenderable.h"
#include "../geometry/Pos3F.h"
#include "../geometry/Vec3F.h"
#include "ColorRGBA.h"

class TestRenderable : public IRenderable {
public:
	TestRenderable() {}
	
	int NumIndices() const override {
		return 36;
	}

	int NumPoints() const override {
		return 24;
	}

	void Render(std::vector<float>& vbo, size_t vboLoc, size_t pointCount, std::vector<unsigned int>& ebo, size_t eboLoc) const override {
		auto WriteEbo = [&ebo, &vbo](size_t eboLoc, size_t indexCount, size_t vboLoc, Pos3F ul, Pos3F ur, Pos3F ll, Pos3F lr,
			ColorRGBA c) {
			ebo[eboLoc] = indexCount;
			ebo[eboLoc + 1] = indexCount + 3;
			ebo[eboLoc + 2] = indexCount + 1;
			ebo[eboLoc + 3] = indexCount;
			ebo[eboLoc + 4] = indexCount + 2;
			ebo[eboLoc + 5] = indexCount + 3;

			Vec3F v1 = ul.Subtract(ur);
			Vec3F v2 = ul.Subtract(ll);
			Vec3F norm = Vec3F::Cross(v2, v1);
			norm.Normalize();

			Pos3F ps[] = { ul, ur, ll, lr };
			for (size_t i = 0; i < 4; i++) {
				vbo[vboLoc + 0 + i * 10] = ps[i].x;
				vbo[vboLoc + 1 + i * 10] = ps[i].y;
				vbo[vboLoc + 2 + i * 10] = ps[i].z;
				vbo[vboLoc + 3 + i * 10] = c.r;
				vbo[vboLoc + 4 + i * 10] = c.g;
				vbo[vboLoc + 5 + i * 10] = c.b;
				vbo[vboLoc + 6 + i * 10] = c.a;
				vbo[vboLoc + 7 + i * 10] = norm.x;
				vbo[vboLoc + 8 + i * 10] = norm.y;
				vbo[vboLoc + 9 + i * 10] = norm.z;
			}
		};

		Pos3F p0(-0.5, -0.5, -0.5);
		Pos3F p1(-0.5, -0.5, 0.5);
		Pos3F p2(-0.5, 0.5, -0.5);
		Pos3F p3(-0.5, 0.5, 0.5);
		Pos3F p4(0.5, -0.5, -0.5);
		Pos3F p5(0.5, -0.5, 0.5);
		Pos3F p6(0.5, 0.5, -0.5);
		Pos3F p7(0.5, 0.5, 0.5);

		WriteEbo(eboLoc, pointCount, vboLoc, p0, p4, p2, p6, ColorRGBA(.5,0,0,1));
		WriteEbo(eboLoc + 6, pointCount + 4, vboLoc + 40, p1, p5, p0, p4, ColorRGBA(0,.5,0,1));
		WriteEbo(eboLoc + 12, pointCount + 8, vboLoc + 40 * 2, p4, p5, p6, p7, ColorRGBA(0,0,.5,1));
		WriteEbo(eboLoc + 18, pointCount + 12, vboLoc + 40*3, p6, p2, p7, p3, ColorRGBA(.5,.5,0,1));
		WriteEbo(eboLoc + 24, pointCount + 16, vboLoc + 40*4, p1, p0, p3, p2, ColorRGBA(0.5, 0, 0.5, 1));
		WriteEbo(eboLoc + 30, pointCount + 20, vboLoc + 40*5, p5, p1, p7, p3, ColorRGBA(0,0.5,0.5,1));
	}
private:
};