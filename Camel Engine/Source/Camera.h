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

public:
	int v_fov, h_fov;
	float aspect_ratio;
	Frustum frustrum;
};

#endif // !_CAMERA_
