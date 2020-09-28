#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Primitive.h"
#include "p2DynArray.h"
#include "PhysBody3D.h"
#include "ModuleSceneIntro.h"

struct PhysVehicle3D;
struct PhysBody3D;

#define MAX_ACCELERATION 2000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 250.0f
#define BULLETS 10

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void CameraFollow();
	void ResetPlayer();

public:

	vec3 position;
	vec3 CamD;
	vec3 BulletD;
	vec3 CarPos;

	p2List<PhysBody3D*> bullets;
	bool camera = true;
	Sphere* proyectile[BULLETS];
	int proyectileAdd = 0;

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;

};