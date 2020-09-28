#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"

class btRigidBody;
class Module;
class vec3;
class Primitive;
class btQuaternion;
// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	const vec3 GetPos() const;
	btRigidBody* GetBody();
	void SetSensor(bool is_sensor);
	void SetRotation(btQuaternion orientation);
	btRigidBody* body = nullptr;

	bool is_sensor;
	bool activate_sensor = false;

	float			rotation;

private:

public:
	p2List<Module*> collision_listeners;
};

#endif // __PhysBody3D_H__