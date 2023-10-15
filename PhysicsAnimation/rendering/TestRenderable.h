#pragma once

#include "Renderable.h"

class TestRenderable : public Renderable {
public:
	TestRenderable() {}
	
	int NumIndices() const override {
		return 36;
	}

	int NumPoints() const override {
		return 8;
	}

	void Render(std::vector<float>& vbo, size_t vboLoc, std::vector<unsigned int>& ebo, size_t eboLoc) const override {
		for (int i = 0; i <= 1; i++) {
			for (int j = 0; j <= 1; j++) {
				for (int k = 0; k <= 1; k++) {
					vbo[vboLoc + (4 * i + 2 * j + k) * 7] = -0.5f + i;
					vbo[vboLoc + (4 * i + 2 * j + k) * 7 + 1] = -0.5f + j;
					vbo[vboLoc + (4 * i + 2 * j + k) * 7 + 2] = -0.5f + k;
					vbo[vboLoc + (4 * i + 2 * j + k) * 7 + 3] = 0.2f;
					vbo[vboLoc + (4 * i + 2 * j + k) * 7 + 4] = 0.6f;
					vbo[vboLoc + (4 * i + 2 * j + k) * 7 + 5] = 0.2f;
					vbo[vboLoc + (4 * i + 2 * j + k) * 7 + 6] = 1.0f;
				}
			}
		}

		auto WriteEbo = [&ebo](size_t eboLoc, int ul, int ur, int ll, int lr) {
			ebo[eboLoc] = ul;
			ebo[eboLoc + 1] = ur;
			ebo[eboLoc + 2] = lr;
			ebo[eboLoc + 3] = ul;
			ebo[eboLoc + 4] = lr;
			ebo[eboLoc + 5] = ll;
		};

		WriteEbo(eboLoc, 0, 4, 2, 6);
		WriteEbo(eboLoc + 6, 1, 5, 0, 4);
		WriteEbo(eboLoc + 12, 4, 5, 6, 7);
		WriteEbo(eboLoc + 18, 6, 2, 7, 3);
		WriteEbo(eboLoc + 24, 1, 0, 3, 2);
		WriteEbo(eboLoc + 30, 5, 1, 7, 3);

		//ebo[eboLoc] = vboLoc;
		//ebo[eboLoc + 1] = vboLoc + 2;
		//ebo[eboLoc + 2] = vboLoc + 4;
		//ebo[eboLoc + 3] = vboLoc;
		//ebo[eboLoc + 4] = vboLoc + 6;
		//ebo[eboLoc + 5] = vboLoc + 4;
		//ebo[eboLoc + 6] = vboLoc;
		//ebo[eboLoc + 7] = vboLoc + 4;
		//ebo[eboLoc + 8] = vboLoc + 5;
		//ebo[eboLoc + 9] = vboLoc;
		//ebo[eboLoc + 10] = vboLoc + 5;
		//ebo[eboLoc + 11] = vboLoc + 1;
		//ebo[eboLoc + 12] = vboLoc;
		//ebo[eboLoc + 13] = vboLoc + 1;
		//ebo[eboLoc + 14] = vboLoc + 2;
		//ebo[eboLoc + 15] = vboLoc;
		//ebo[eboLoc + 16] = vboLoc + 2;
		//ebo[eboLoc + 17] = vboLoc + 3;
		//ebo[eboLoc + 18] = vboLoc + 4;
		//ebo[eboLoc + 19] = vboLoc + 6;
		//ebo[eboLoc + 20] = vboLoc + 7;
		//ebo[eboLoc + 21] = vboLoc + 4;
		//ebo[eboLoc + 22] = vboLoc + 7;
		//ebo[eboLoc + 23] = vboLoc + 5;
		//ebo[eboLoc + 24] = vboLoc + 5;
		//ebo[eboLoc + 25] = vboLoc + 1;
		//ebo[eboLoc + 26] = vboLoc + 3;
		//ebo[eboLoc + 27] = vboLoc + 5;
		//ebo[eboLoc + 28] = vboLoc + 3;
		//ebo[eboLoc + 29] = vboLoc + 7;
		//ebo[eboLoc + 30] = vboLoc + 3;
		//ebo[eboLoc + 31] = vboLoc + 2;
		//ebo[eboLoc + 32] = vboLoc + 6;
		//ebo[eboLoc + 33] = vboLoc + 3;
		//ebo[eboLoc + 34] = vboLoc + 6;
		//ebo[eboLoc + 35] = vboLoc + 7;
	}
private:
};