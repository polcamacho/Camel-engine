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
	editor_cam = new Camera();

	Position = float3(5.0f, 10.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);

	background = { 0.12f, 0.12f, 0.12f, 1.0f };
	constant_mov = 0.9f;
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
	editor_cam->frustum.Translate(Position);
	editor_cam->LookAt(Position);
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


	float3 newPos(0.0f, 0.0f, 0.0f);
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
		newPos += editor_cam->frustum.front * move_speed * speed_multiplier * dt * constant_mov;
	if ((App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) || (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)) 
		newPos -= editor_cam->frustum.front * move_speed * speed_multiplier * dt * constant_mov;
	//Left/Right
	if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) || (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)) 
		newPos += editor_cam->frustum.WorldRight() * move_speed * speed_multiplier * dt * constant_mov;
	if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) || (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT))
		newPos -= editor_cam->frustum.WorldRight() * move_speed * speed_multiplier * dt * constant_mov;

	//Drag
	if ((App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT))
	{
		newPos -= editor_cam->frustum.WorldRight() * App->input->GetMouseXMotion() * speed_multiplier * drag_speed * dt * constant_mov;
		newPos += editor_cam->frustum.up * App->input->GetMouseYMotion() * speed_multiplier * drag_speed * dt * constant_mov;
	}

	// Zoom 

	if (App->input->GetMouseZ() > 0)
		newPos += editor_cam->frustum.front * zoom_speed * dt * constant_mov;
	else if(App->input->GetMouseZ() < 0 )
		newPos -= editor_cam->frustum.front * zoom_speed * dt * constant_mov;

	// Mouse motion ----------------

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			Quat rotx(editor_cam->frustum.up, dx * dt * (constant_mov / 3));
			Quat roty(editor_cam->frustum.WorldRight(), dy * dt * (constant_mov / 3));

			float3 distance = Position - Reference;
			distance = rotx.Transform(distance);
			distance = roty.Transform(distance);

			Position = distance + Reference;
			LookAt(Reference);

		}
	}

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		if (dx != 0)
		{
			Quat quat = Quat::RotateY(dt * dx * (constant_mov / 3));
			editor_cam->frustum.WorldRight() = quat.Mul(editor_cam->frustum.WorldRight()).Normalized();
			editor_cam->frustum.front = quat.Mul(editor_cam->frustum.front).Normalized();
			editor_cam->frustum.up = quat.Mul(editor_cam->frustum.up).Normalized();
		}

		if (dy != 0)
		{
			Quat quat = Quat::RotateAxisAngle(editor_cam->frustum.WorldRight(), dt * dy * (constant_mov / 3));
			editor_cam->frustum.up = quat.Mul(editor_cam->frustum.up).Normalized();
			editor_cam->frustum.front = quat.Mul(editor_cam->frustum.front).Normalized();

		}

	}

	Position += newPos;
	Reference += newPos;
	editor_cam->frustum.pos = Position;
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(float3& Spot)
{
	editor_cam->LookAt(Spot);
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