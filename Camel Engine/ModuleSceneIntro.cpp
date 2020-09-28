#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	for (int i = 0; i < CUBES; i++) {
		map_list[i] = nullptr;
	}

}

ModuleSceneIntro::~ModuleSceneIntro()
{}

bool ModuleSceneIntro::Awake() {

	bool ret = true;

	node = LoadCircuit(map_file);
	
	pugi::xml_node cube;

	for (cube = node.child("cube_size"); cube && ret; cube = cube.next_sibling("cube_size")) {

		sizex = cube.attribute("x").as_int();
		sizey = cube.attribute("y").as_int();
		sizez = cube.attribute("z").as_int();
		LOG("%i %i %i", sizex, sizey, sizez);
	}

	for (cube = node.child("box"); cube && ret; cube = cube.next_sibling("box")) {
		
		Cube* box = new Cube(sizex, sizey+1, sizez);

		x = cube.attribute("x").as_float();
		y = cube.attribute("y").as_float();
		z = cube.attribute("z").as_float();
		rot = cube.attribute("rotation").as_int();

		LOG("%f %f %f %i", x, y, z, rot);
		
		if (cont % 2 == 0) {

			box->color.r = 0.7;
			box->color.g = 0;
			box->color.b = 0;

		}
		
		else {

			box->color.r = 0;
			box->color.g = 0;
			box->color.b = 0.7;

		}

		box->SetPos(x, y + box->size.y*0.5, z);
		box->SetRotation(rot, { 0,1,0 });
	
		map_list[cubesAdd] = box;

		cubesAdd++;
		cont++;
	}
	
	return ret;
}

// Load assets
bool ModuleSceneIntro::Start()
{

	CreateHinges({0,0,-7}, { 10,3,1 });
	CreateHinges({ 30,0,97.5f }, { 1,3,12});

	LOG("Loading Intro assets");
	bool ret = true;

	App->audio->PlayMusic("Audio/barca_anthem.ogg");

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	for (int i = 0; i < CUBES; i++) {
		if (map_list[i] != nullptr)
		{
			Cube aux_cube = *map_list[i];
			App->physics->AddBody(aux_cube, 1000000);

		}
	}

	ramp = new Cube(14, 0.1, 10);
	ramp->color = Gold;
	ramp->SetPos(-55.5f, 1.25f, 5);
	ramp->SetRotation(30, { -1, 0, 0});

	Cube aux_ramp = *ramp;
	App->physics->AddBody(aux_ramp, 0);

	ramp2 = new Cube(10, 0.1, 14);
	ramp2->color = Gold;
	ramp2->SetPos(-20, 1.23f, 97.5f);
	ramp2->SetRotation(25, {0,0,1 });

	Cube aux_ramp2 = *ramp2;
	App->physics->AddBody(aux_ramp2, 0);

	CheckPoint({ 0,2,20 }, 0, { 0,0,1 });
	CheckPoint2({ 0,2,97 }, -90,{0, 1, 0});
	CheckPoint3({ 20,2,-92 }, 135, { 0, 1, 0 });


	App->audio->PlayMusic("Audio/avicii.ogg");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	for (int i = 0; i < CUBES; i++) {
		
		if (map_list[i] != nullptr) {
			
			delete map_list[i];

		}

	}

	delete ramp;
	delete ramp2;

	
	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	/*Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();*/
	Cube floor(600, 2, 600);
	floor.SetPos(0, -1, 0);
	floor.color = Cyan;
	floor.Render();

	Cube roof(600, 2, 600);
	roof.SetPos(0, 100, 0);
	roof.color = Cyan;
	roof.Render();

	Cube wall_left(2, 100, 600);
	wall_left.SetPos(-300, 50, 0);
	wall_left.color = Cyan;
	wall_left.Render();

	Cube wall_right(2, 100, 600);
	wall_right.SetPos(300, 50, 0);
	wall_right.color = Cyan;
	wall_right.Render();

	Cube wall_front(600, 100, 2);
	wall_front.SetPos(0, 50, -300);
	wall_front.color = Cyan;
	wall_front.Render();

	Cube wall_back(600, 100, 2);
	wall_back.SetPos(0, 50, 300);
	wall_back.color = Cyan;
	wall_back.Render();

	if (pb_cubes.Count() != 0 && s_cubes.Count() != 0 && s_cubes.Count() == pb_cubes.Count()) {
		for (int i = 0; i < s_cubes.Count(); i++) {
			pb_cubes[i]->GetTransform(&s_cubes[i].transform);
			s_cubes[i].Render();
		}

	}

	for (int i = 0; i < CUBES; i++) {

		if (map_list[i] != nullptr)
		{
			map_list[i]->Render();
		}
	}

	ramp->Render();
	ramp2->Render();

	float race_time = 0;

	if (App->T.d == false) {
		race_time = App->T.Read();
		race_time = race_time / 1000.0f;
	}

	char title[80];
	sprintf_s(title, "Velocity: %.1f Km/h Time: %.2f Current lap: %i", App->player->vehicle->GetKmh(), race_time, lap);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}


void ModuleSceneIntro::CheckPoint(const vec3 pos, float rotation, vec3 direction)
{
	cube_sensor.size.x = 14;
	cube_sensor.size.y = 4;
	cube_sensor.size.z = 0.1f;
	cube_sensor.SetPos(pos.x, pos.y, pos.z);
	cube_sensor.SetRotation(rotation, direction);
	check_p = App->physics->AddBody(cube_sensor, 0);
	check_p->GetTransform(&cube_sensor.transform);
	check_p->SetSensor(true);
	check_p->collision_listeners.add(this);
	check_p->is_sensor = true;
}

void ModuleSceneIntro::CheckPoint2(const vec3 pos, float rotation, vec3 direction)
{
	cube_sensor2.size.x = 10;
	cube_sensor2.size.y = 4;
	cube_sensor2.size.z = 0.1f;
	cube_sensor2.SetPos(pos.x, pos.y, pos.z);
	cube_sensor2.SetRotation(rotation, direction);
	check_p2 = App->physics->AddBody(cube_sensor2, 0);
	check_p2->GetTransform(&cube_sensor2.transform);
	check_p2->SetSensor(true);
	check_p2->collision_listeners.add(this);
	check_p2->is_sensor = true;
}

void ModuleSceneIntro::CheckPoint3(const vec3 pos, float rotation, vec3 direction)
{
	cube_sensor3.size.x = 10;
	cube_sensor3.size.y = 4;
	cube_sensor3.size.z = 0.1f;
	cube_sensor3.SetPos(pos.x, pos.y, pos.z);
	cube_sensor3.SetRotation(rotation, direction);
	check_p2 = App->physics->AddBody(cube_sensor3, 0);
	check_p2->GetTransform(&cube_sensor3.transform);
	check_p2->SetSensor(true);
	check_p2->collision_listeners.add(this);
	check_p2->is_sensor = true;
}



void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if ((body1 == check_p || body1 == check_p2 || body1 == check_p3) && body2 == (PhysBody3D*)App->player->vehicle)
	{
		newpos = App->player->vehicle->GetPos();
		sensor = true;
		LOG("PASA");
		
	}

}

pugi::xml_node ModuleSceneIntro::LoadCircuit(pugi::xml_document& map_file) const
{
	
	pugi::xml_node ret;

	pugi::xml_parse_result result = map_file.load_file("Map_Cube_Coords.xml");

	if (result == NULL) { 
		
		LOG("Could not load map xml file map.xml. pugi error: %s", result.description()); 

	}

	else {

		ret = map_file.child("map");

	}

	return ret;

}

void ModuleSceneIntro::CreateHinges(vec3 pos, vec3 size) {

	Cube s_cube;
	PhysBody3D* pb_cube;

	s_cube.Size(size.x, size.y, size.z);
	s_cube.color = Red;
	s_cubes.PushBack(s_cube);
	pb_cube = App->physics->AddBody(s_cube, 3);
	pb_cube->SetPos(pos.x, pos.y, pos.z);
	App->physics->AddConstraintSlider(*pb_cube, true);
	pb_cubes.PushBack(pb_cube);
	pb_cube->GetBody()->setActivationState(1);

}



