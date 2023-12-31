#pragma once

#include "../rendering/IRenderable.h"
#include "../geometry/Pos3F.h"

class PlaneRenderable :
    public IRenderable
{
public:
	PlaneRenderable(const Pos3F& p1, const Pos3F& p2, const Pos3F& p3, const Pos3F& p4, const Material& mat);
	int NumIndices() const override;
	int NumPoints() const override;
	void Render(std::vector<float>& vbo, size_t vboLoc, size_t pointCount, std::vector<unsigned int>& ebo, size_t eboLoc) const override;
	Material GetMaterial() const override { return material; }
private:
	Material material;
	Pos3F p1, p2, p3, p4;
};

