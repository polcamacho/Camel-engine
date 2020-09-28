#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "PugiXml\src\pugixml.hpp"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"

#define MAX_SNAKE 2
#define CUBES 500
#define SPHERES 100
#define SPHERES2 100
#define RAMPS 10


struct PhysMotor3D;
struct PhysBody3D;
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();
	
	bool Awake();
	bool Start();
	update_status Update(float dt);
	pugi::xml_node LoadCircuit(pugi::xml_document&) const;
	bool CleanUp();
	void CheckPoint(const vec3 position, float rotation, vec3 direction);
	void CheckPoint2(const vec3 position, float rotation, vec3 direction);
	void CheckPoint3(const vec3 position, float rotation, vec3 direction);

	void CreateHinges(vec3 pos, vec3 size);
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	
public:
	
	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	Cube* map_list[CUBES];
	Cube* ramp;
	Cube* ramp2;
	PhysBody3D* bodyA = nullptr;
	PhysBody3D* bodyB = nullptr;

	//checkpoints
	Cube cube_sensor;
	PhysBody3D* check_p;
	Cube cube_sensor2;
	PhysBody3D* check_p2;
	Cube cube_sensor3;
	PhysBody3D* check_p3;
	vec3 newpos;
	bool sensor=false;

	//laps
	int lap = 1;

	
	Sphere* sphere_list[SPHERES];
	Sphere* sphere_list2[SPHERES2];

	pugi::xml_document map_file;
	pugi::xml_node node;

	float x = 0, y = 0, z = 0;
	int sizex = 0, sizey = 0, sizez = 0, rot = 0;
	int cont = 0;
	int cubesAdd = 0;
	int rampsAdd = 0;


	p2DynArray<PhysBody3D*> pb_cubes;
	p2DynArray<Cube>s_cubes;

};

