#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleSceneIntro.h"


ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;

	for (int i = 0; i < BULLETS; i++) {
		proyectile[i] = nullptr;
	}

}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 0.45, 3);
	car.chassis_offset.Set(0, 1, 0);

	car.lateral_right_size.Set(0.1, 0.8, 3);
	car.lateral_right_offset.Set(0.95, 1.3, 0);

	car.lateral_left_size.Set(0.1, 0.8, 3);
	car.lateral_left_offset.Set(-0.95, 1.3, 0);

	car.front_size.Set(2, 0.4, 0.1);
	car.front_offset.Set(0, 1.3, 1.45);

	car.back_size.Set(2, 0.4, 0.1);
	car.back_offset.Set(0, 1.3, -1.45);

	car.seat_size.Set(1, 0.1, 1);
	car.seat_offset.Set(0, 1.3, -0.95);

	car.seatback_size.Set(1, 0.8, 0.15);
	car.seatback_offset.Set(0, 1.8, -1.4);

	car.handlebar_size.Set(0.2, 1.3, 0.2);
	car.handlebar_offset.Set(0, 1.5, 0.9);

	car.handles_left_size.Set(0.4, 0.1, 0.1);
	car.handles_left_offset.Set(0.3, 2, 0.9);

	car.handles_right_size.Set(0.4, 0.1, 0.1);
	car.handles_right_offset.Set(-0.3, 2, 0.9);

	car.aileron_left_size.Set(0.3, 1.3, 0.4);
	car.aileron_left_offset.Set(-0.7, 1.45, -1.6);

	car.aileron_right_size.Set(0.3, 1.3, 0.4);
	car.aileron_right_offset.Set(0.7, 1.45, -1.6);

	car.aileron_size.Set(1.7, 0.1, 0.4);
	car.aileron_offset.Set(0, 1.9, -1.9);

	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 35;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.33f;
	float wheel_width = 0.38f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 3;
	car.wheels = new Wheel[3];

	// FRONT ------------------------
	car.wheels[0].connection.Set(0, connection_height, half_length);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[1].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = false;
	car.wheels[1].drive = false;
	car.wheels[1].brake = true;
	car.wheels[1].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[2].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;
	
	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(-55, 0, 45);
	


	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	for (int i = 0; i < BULLETS; i++) {

		if (proyectile[i] != nullptr) {

			delete proyectile[i];

		}

	}

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;

		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			acceleration *= 2;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
		if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
		if (turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
		}
	}

	else if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		acceleration = -MAX_ACCELERATION;

		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			acceleration *= 2;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}
		
	}

	else{
		
		if (vehicle->GetKmh() > 0.0f)
			brake = BRAKE_POWER/6;

	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT)
	{

			ResetPlayer();

		
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		

			vehicle->SetPos(App->scene_intro->newpos.x, App->scene_intro->newpos.y, App->scene_intro->newpos.z);
			btQuaternion orientation = { 0, 0, 0, 1 };
			vehicle->SetRotation(orientation);
			vehicle->GetBody()->setAngularVelocity({ 0, 0, 0 });
			vehicle->GetBody()->setLinearVelocity({ 0, 0, 0 });
			



	}

	CarPos = vehicle->GetPos();
	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	CameraFollow();

	for (int i = 0; i < bullets.count(); i++)
	{
		Sphere bullet;
		bullet.radius = 0.5f;

		PhysBody3D* bullet_render;
		bullets.at(i, bullet_render);
		vec3 pos = bullet_render->GetPos();
		bullet.SetPos(pos.x, pos.y, pos.z);
		bullet.Render();
	}

	vehicle->Render();

	return UPDATE_CONTINUE;

}
update_status ModulePlayer::PostUpdate(float dt) {

	if (App->input->GetMouseButton(1) == KEY_DOWN)
	{

		//FX
		vec3 pos= vehicle->GetPos();
		vec3 fvector = vehicle->ForwardVector();
		
		PhysBody3D* bullet = App->physics->Throw({ pos.x, pos.y, pos.z }, { fvector.x, fvector.y, fvector.z });

		bullets.add(bullet);

		if (bullets.count() > BULLETS)
		{
			App->physics->UnloadBullet(bullets.getFirst()->data);
			bullets.del(bullets.getFirst());
		}


	}
	
	return UPDATE_CONTINUE;

}

void  ModulePlayer::CameraFollow()
{

	CamD = { -9.0f, 5.0f, -9.0f };
	vec3 fvector = vehicle->ForwardVector();
	vec3 NCam = { CarPos.x + (fvector.x * CamD.x), CarPos.y + (fvector.y + CamD.y), CarPos.z + (fvector.z * CamD.z) };
	vec3 CamPos = App->camera->Position + (NCam - App->camera->Position);

	App->camera->Look(CamPos, CarPos);
}

void ModulePlayer::ResetPlayer()
{

	vehicle->SetPos(0, 0, -10);
	btQuaternion orientation = { 0, 0, 0, 1 };
	vehicle->SetRotation(orientation);

	vehicle->GetBody()->setAngularVelocity({ 0, 0, 0 });
	vehicle->GetBody()->setLinearVelocity({ 0, 0, 0 });
	

	while (bullets.count() > 0)
	{
		App->physics->UnloadBullet(bullets.getFirst()->data);
		bullets.del(bullets.getFirst());
	}
	
}


