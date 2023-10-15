#pragma once
#include "../geometry/Pos3F.h"

class Camera
{
public:
	Camera(float zoom, float aspect, const Pos3F& startPos = Pos3F(0,0,0));

	void SetPosition(const Pos3F& pos);
	Pos3F GetPos() const { return pos; }
	Pos3F GetViewportTrans() const { return Pos3F(zoom, zoom * aspect, 0); }
	void IncreaseZoom(float inc);
	void SetAspect(float w, float h) { aspect = w / h; }
	void Translate(const Vec3F& vel, float dt);
	//void RotateX(float w, float dt);
	//void RotateY(float w, float dt);
	//void RotateZ(float w, float dt);

private:
	Pos3F pos;
	float zoom;
	float aspect;

	void ClampPosition();
};

