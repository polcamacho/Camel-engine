#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Component.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

class GnMesh;
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
	void LookAt(float3& Spot);
	void CullingObjects(GameObject* go);
	bool ContainsAaBox(AABB refBox);

public:
	Frustum frustum;
	float aspect_ratio;
	const char* name;

private:
};

#endif // !_CAMERA_
