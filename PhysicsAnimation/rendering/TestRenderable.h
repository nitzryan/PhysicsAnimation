#pragma once

#include "Renderable.h"

class TestRenderable : public Renderable {
public:
	TestRenderable() {}
	
	int NumIndices() const override {
		return 6;
	}

	int NumPoints() const override {
		return 4;
	}

	void Render(std::vector<float>& vbo, size_t vboLoc, std::vector<unsigned int>& ebo, size_t eboLoc) const override {
		for (int i = 0; i <= 1; i++) {
			for (int j = 0; j <= 1; j++) {
				vbo[vboLoc + (2 * i + j) * 7] = -0.5f + i;
				vbo[vboLoc + (2 * i + j) * 7 + 1] = -0.5f + j;
				vbo[vboLoc + (2 * i + j) * 7 + 2] = -0.5f + 1;
				vbo[vboLoc + (2 * i + j) * 7 + 3] = .5;
				vbo[vboLoc + (2 * i + j) * 7 + 4] = 0.5f;
				vbo[vboLoc + (2 * i + j) * 7 + 5] = 0.5f;
				vbo[vboLoc + (2 * i + j) * 7 + 6] = 1.0f;
			}
		}

		ebo[eboLoc] = vboLoc;
		ebo[eboLoc + 1] = vboLoc + 1;
		ebo[eboLoc + 2] = vboLoc + 3;
		ebo[eboLoc + 3] = vboLoc;
		ebo[eboLoc + 4] = vboLoc + 3;
		ebo[eboLoc + 5] = vboLoc + 2;
	}
private:
};