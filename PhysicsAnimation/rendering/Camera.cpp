#include "Camera.h"

Camera::Camera(float zoom, float aspect, const Pos3F& startPos) :
	pos(startPos), zoom(zoom), aspect(aspect)
{
	ClampPosition();
}

void Camera::SetPosition(const Pos3F& p)
{
	pos = p;
	ClampPosition();
}

void Camera::IncreaseZoom(float inc)
{
	zoom += inc;
	if (zoom < 0.01)
		zoom = 0.01;
	else if (zoom > 10)
		zoom = 10;
}

void Camera::Translate(const Vec3F& vel, float dt)
{
	auto displacement = Vec3F::Mul(vel, dt);
	pos.Add(displacement);
	ClampPosition();
}

void Camera::ClampPosition()
{
	//No clamping for now
}
