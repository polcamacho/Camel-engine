#include "Camera.h"

Camera::Camera():Component()
{
	v_fov = 9;
	h_fov = 16;
	aspect_ratio = (tan(v_fov / 2)) / (tan(h_fov / 2));
	
	frustrum.farPlaneDistance = 200.0f;
	frustrum.nearPlaneDistance = 1.0f;
}

Camera::~Camera()
{

}

void Camera::Update(float dt)
{

}
