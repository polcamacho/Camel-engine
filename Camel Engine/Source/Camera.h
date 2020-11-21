#pragma once
#ifndef _CAMERA_
#define _CAMERA_

#include "Component.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

class Camera: public Component{

public:
	Camera();
	~Camera();
	void Update(float dt);
	void OnEditor() override;

	float GetNearPlane();
	float GetFarPlane();
	float GetAspectRatio();

	float GetFov();

	void DrawCameraLines();

public:
	Frustum frustum;
	const char* name;
};

#endif // !_CAMERA_
