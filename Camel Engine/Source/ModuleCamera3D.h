#ifndef _MODULE_CAMERA_3D_H_
#define _MODULE_CAMERA_3D_H_

#include "Module.h"
#include "Globals.h"
#include "Color.h"
#include "MathGeoLib/include/MathGeoLib.h"

class Camera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Init();
	bool Start();
	bool LoadConfig(JSON_Object* config) override;
	update_status Update(float dt);
	bool CleanUp();

	void LookAt(const float3& Spot);
	void Move(const float3& Movement);

	void Reset();
	void SetBackgroundColor(float r, float g, float b, float w);

public:

	float3 Position, Reference;
	Color background;
	Camera* cam;

	float move_speed;
	float drag_speed;
	float zoom_speed;
	float sensitivity;
	float constant_mov;

};

#endif // !_MODULE_CAMERA_3D_H_