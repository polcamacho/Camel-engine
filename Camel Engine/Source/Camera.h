#pragma once
#ifndef _CAMERA_
#define _CAMERA_

#include "Component.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

class Camera: public Component{

public:
	Camera();
	~Camera();
	virtual void Update() override;
	virtual void OnEditor() override;

	float GetNearPlane();
	float GetFarPlane();
	float GetAspectRatio();
	float GetFOV();

	void SetNearPlane(float nearP);
	void SetFarPlane(float farP);
	void SetFOV(float vFOV);
	void SetAspectRatio(float a_ratio);

	float* GetProjectionMatrix();
	float* GetViewMatrix();

	void DrawCameraLines();

public:
	Frustum frustum;
	float aspect_ratio;
	const char* name;
};

#endif // !_CAMERA_
