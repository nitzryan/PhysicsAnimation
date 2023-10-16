#pragma once

#include "../geometry/Pos3F.h"
#include "ColorRGBA.h"
#include <vector>

class BufferWriter {
public:
	BufferWriter() = delete;

	static void AddPoint(std::vector<float>& vbo, size_t& vboLoc,
		const Pos3F& p, const ColorRGBA& c, const Vec3F& n) {
		vbo[vboLoc] = p.x;
		vbo[vboLoc + 1] = p.y;
		vbo[vboLoc + 2] = p.z;
		vbo[vboLoc + 3] = c.r;
		vbo[vboLoc + 4] = c.g;
		vbo[vboLoc + 5] = c.g;
		vbo[vboLoc + 6] = c.a;
		vbo[vboLoc + 7] = n.x;
		vbo[vboLoc + 8] = n.y;
		vbo[vboLoc + 9] = n.z;
		vboLoc += 10;
	}
};