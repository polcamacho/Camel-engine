#ifndef _MODULE_CAMERA_3D_H_
#define _MODULE_CAMERA_3D_H_

#include "Module.h"
#include "Globals.h"
#include "Color.h"
#include "MathGeoLib/include/MathGeoLib.h"

class Camera;
class GameObject;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Init();
	bool Start();
	bool LoadConfig(JSON_Object* config) override;
	update_status Update(float dt);
	void DrawRay();
	bool CleanUp();

	void LookAt(float3& Spot);
	void Move(const float3& Movement);

	void Reset();
	void SetBackgroundColor(float r, float g, float b, float w);

	void PickMouse();
	//void ReturnObjectToPick(LineSegment ray);
	void PickObject(LineSegment ray);

public:

	float3 Position, Reference;
	Color background;
	Camera* editor_cam;
	GameObject* gameObject;
	GameObject* children;
	std::vector<GameObject*> vec_objects;
	LineSegment ray;

	float move_speed;
	float drag_speed;
	float zoom_speed;
	float sensitivity;
	float constant_mov;

	bool show_raycast;

};

#endif // !_MODULE_CAMERA_3D_H_