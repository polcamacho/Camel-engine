#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

#include "parson/parson.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	name = "camera";
	cam = new Camera();

	Position = float3(5.0f, 10.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);

	background = { 0.12f, 0.12f, 0.12f, 1.0f };
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Init()
{
	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;
	cam->frustum.Translate(Position);
	return ret;
}

bool ModuleCamera3D::LoadConfig(JSON_Object* config)
{
	move_speed = json_object_get_number(config, "move_speed");
	drag_speed = json_object_get_number(config, "drag_speed");
	zoom_speed = json_object_get_number(config, "zoom_speed");
	sensitivity = json_object_get_number(config, "sensitivity");

	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	if (!App->editor->IsSceneFocused())
		return UPDATE_CONTINUE;


	float3 newPos(0, 0, 0);
	int speed_multiplier = 1;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed_multiplier = 2;

	// Up/Down
	//if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += move_speed * dt;
	//if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= move_speed * dt;

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) 
	{
		if (App->scene->selectedGameObject != nullptr)
		{
			float3 center((App->scene->selectedGameObject->GetTransform()->GetPosition().x,
				App->scene->selectedGameObject->GetTransform()->GetPosition().y,
				App->scene->selectedGameObject->GetTransform()->GetPosition().z));

				LookAt(center);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		Reset();
	}

	//Forwards/Backwards
	if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) || (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)) 
		newPos += cam->frustum.front * move_speed * speed_multiplier * dt;
	LOG("%d %d %d", newPos);
	if ((App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) || (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)) 
		newPos -= cam->frustum.front * move_speed * speed_multiplier * dt;

	//Left/Right
	if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) || (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)) 
		newPos += cam->frustum.WorldRight() * move_speed * speed_multiplier * dt;
	if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) || (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT))
		newPos -= cam->frustum.WorldRight() * move_speed * speed_multiplier * dt;

	//Drag
	if ((App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT))
	{
		newPos -= cam->frustum.WorldRight() * App->input->GetMouseXMotion() * speed_multiplier * drag_speed * dt;
		newPos += cam->frustum.up * App->input->GetMouseYMotion() * speed_multiplier * drag_speed * dt;
	}

	// Zoom 

	if (App->input->GetMouseZ() > 0)
		newPos += cam->frustum.front * zoom_speed * dt;
	else if(App->input->GetMouseZ() < 0 )
		newPos -= cam->frustum.front * zoom_speed * dt;

	// Mouse motion ----------------

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			if (dx != 0)
			{
				Quat quat = Quat::RotateY(dt * dx);
				cam->frustum.WorldRight() = quat.Mul(cam->frustum.WorldRight()).Normalized();
				cam->frustum.front = quat.Mul(cam->frustum.front).Normalized();
				cam->frustum.up = quat.Mul(cam->frustum.up).Normalized();
			}

			if (dy != 0)
			{
				Quat quat = Quat::RotateAxisAngle(cam->frustum.WorldRight(), dt * dy);
				cam->frustum.up = quat.Mul(cam->frustum.up).Normalized();
				cam->frustum.front = quat.Mul(cam->frustum.front).Normalized();

			}

		}
	}

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		if (dx != 0)
		{
			Quat quat = Quat::RotateY(dt * dx);
			cam->frustum.WorldRight() = quat.Mul(cam->frustum.WorldRight()).Normalized();
			cam->frustum.front = quat.Mul(cam->frustum.front).Normalized();
			cam->frustum.up = quat.Mul(cam->frustum.up).Normalized();
		}

		if (dy != 0)
		{
			Quat quat = Quat::RotateAxisAngle(cam->frustum.WorldRight(), dt * dy);
			cam->frustum.up = quat.Mul(cam->frustum.up).Normalized();
			cam->frustum.front = quat.Mul(cam->frustum.front).Normalized();

		}

	}

	Position += newPos;
	Reference += newPos;
	cam->frustum.pos = Position;
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const float3& Spot)
{
	Reference = Spot;

	float3 direction = Reference - cam->frustum.pos;
	float3x3 matrix = float3x3::LookAt(cam->frustum.front, direction.Normalized(), cam->frustum.up, float3(0, 1, 0));

	cam->frustum.front = matrix.MulDir(cam->frustum.front).Normalized();
	cam->frustum.up = matrix.MulDir(cam->frustum.up).Normalized();

}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3& Movement)
{
	Position += Movement;
	Reference += Movement;

}

// -----------------------------------------------------------------

void ModuleCamera3D::Reset()
{
	Position = float3(0.0f, 0.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
	LookAt(float3(0, 0, 0));
}

void ModuleCamera3D::SetBackgroundColor(float r, float g, float b, float w)
{
	background = { r,g,b,w };
}