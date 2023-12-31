#pragma once
#include <vector>
#include "./geometry/Pos3F.h"
#include "./geometry/Vec3F.h"
#include "rendering/ColorRGBA.h"
#include "./rendering/IRenderable.h"

class SphereRenderable :
	public IRenderable{
public:
	SphereRenderable(Pos3F center, float radius);
	int NumIndices() const override { return 6 * (thetaSlices + 1) * phiSlices; };
	int NumPoints() const override { return (thetaSlices + 1) * (phiSlices + 1); }; // need 0 and 2 * pi for theta, since different text coords
	void Render(std::vector<float>& vbo, size_t vboLoc, size_t pointCount, std::vector<unsigned int>& ebo, size_t eboLoc) const override;
	Material GetMaterial() const override;
	//void Update(float dt); // porbbaly not needed as it will not move
	float get_radius() const;
	Pos3F get_center() const;
	void SetThetaSlices(int s);
	void SetPhiSlices(int s);
private:
	Pos3F center;
	float radius;
	ColorRGBA color;
	int thetaSlices;
	int phiSlices;
	
	std::vector<Pos3F> renderPoints;
	std::vector<Vec3F> normals;
	void GenerateRenderPoints();
};