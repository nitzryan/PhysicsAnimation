#include "Camera.h"

Camera::Camera(float zoom, float aspect, const Pos3F& startPos, const Vec3F& startLook, const Vec3F& upDir) :
	pos(startPos), zoom(zoom), aspect(aspect), lookDir(startLook), upDir(upDir), rot(1.0f)
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
	auto v = glm::vec3(displacement.x, displacement.y, displacement.z);
	v = rot * v;
	displacement.x = v.x;
	displacement.y = v.y;
	displacement.z = v.z;

	pos.Add(displacement);
	ClampPosition();
}

void Camera::RotateX(float w, float dt)
{
	// Rotation Matrix 
	float theta = w * dt;
	float cosTheta = cosf(theta);
	float sinTheta = sinf(theta);

	// SinTheta above needs to be flipped for this matrix
	glm::mat3 rx = glm::mat3(
		glm::vec3(1, 0, 0),
		glm::vec3(0, cosTheta, sinTheta),
		glm::vec3(0, -sinTheta, cosTheta)
	);
	rot = rx * rot;

	// Modify lookDir
	float tmp = lookDir.y;
	lookDir.y = lookDir.y * cosTheta - lookDir.z * sinTheta;
	lookDir.z = tmp * sinTheta + lookDir.z * cosTheta;

	// Modify upDir
	tmp = upDir.y;
	upDir.y = upDir.y * cosTheta - upDir.z * sinTheta;
	upDir.z = tmp * sinTheta + upDir.z * cosTheta;
}

void Camera::RotateY(float w, float dt)
{
	// Rotation Matrix 
	float theta = w * dt;
	float cosTheta = cosf(theta);
	float sinTheta = sinf(theta);

	// SinTheta above needs to be flipped for this matrix
	glm::mat3 ry = glm::mat3(
		glm::vec3(cosTheta, 0, -sinTheta),
		glm::vec3(0, 1, 0),
		glm::vec3(sinTheta, 0, cosTheta)
	);
	rot = ry * rot;

	// Modify lookDir
	float tmp = lookDir.x;
	lookDir.x = lookDir.x * cosTheta + lookDir.z * sinTheta;
	lookDir.z = -tmp * sinTheta + lookDir.z * cosTheta;
	

	// Modify upDir
	tmp = upDir.x;
	upDir.x = upDir.x * cosTheta + upDir.z * sinTheta;
	upDir.z = -tmp * sinTheta + upDir.z * cosTheta;

	// Prevent small errors from accumulating, even though in pure math these vectors should be normal
	lookDir.Normalize();
	upDir.Normalize();
}

void Camera::ClampPosition()
{
	//No clamping for now
}
