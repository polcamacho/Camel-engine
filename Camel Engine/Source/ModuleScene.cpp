#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "parson/parson.h"
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "Time.h"

#include "ImGuizmo/ImGuizmo.h"

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled), show_grid(true), selectedGameObject(nullptr), root(nullptr) 
{
	name = "scene";
	main_camera = nullptr;
}

ModuleScene::~ModuleScene() {}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	ImGuizmo::Enable(true);

	App->camera->Move(float3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(float3(0, 0, 0));

	root = new GameObject();
	selectedGameObject = root;
	root->SetName("Root");

	GameObject* house = MeshImporter::LoadFBX("Assets/Models/baker_house/BakerHouse.FBX");
	AddGameObject(house);

	CreateMainCamera();
	//GameObject* street = MeshImporter::LoadFBX("Assets/Models/Street/street2.FBX");
	//AddGameObject(street);
		
	return ret;
}

bool ModuleScene::Init()
{
	return true;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	root->DeleteChildren();
	delete root;
	root = nullptr;

	selectedGameObject = nullptr;

	return true;
}

void ModuleScene::AddGameObject(GameObject* gameObject)
{
	gameObject->SetParent(root);
	root->AddChild(gameObject);

	selectedGameObject = gameObject;
}

void ModuleScene::DeleteGameObject(GameObject* gameObject)
{
	if (root->RemoveChild(gameObject))
	{
		gameObject->DeleteChildren();
	}
	else if (gameObject->GetParent()->RemoveChild(gameObject))
	{
		gameObject->DeleteChildren();
	}

	delete gameObject;
	gameObject = nullptr;
}

void ModuleScene::SetDroppedTexture(GnTexture* texture)
{
	if (selectedGameObject != nullptr) 
	{
		if (selectedGameObject->GetComponent(ComponentType::MESH) == nullptr) 
		{
			delete texture;
			texture = nullptr;
			return;
		}

		Material* material = dynamic_cast<Material*>(selectedGameObject->GetComponent(ComponentType::MATERIAL));

		if (material == nullptr)
		{
			material = dynamic_cast<Material*>(selectedGameObject->AddComponent(ComponentType::MATERIAL));
		}

		material->SetTexture(texture);
	}
}

bool ModuleScene::LoadConfig(JSON_Object* config)
{
	show_grid = json_object_get_boolean(config, "show_grid");

	return true;
}

// Update: draw background
update_status ModuleScene::Update(float dt)
{
	if (show_grid) 
	{
		GnGrid grid(24);
		grid.Render();
	}

	root->Update();

	if ((App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN) && (selectedGameObject != nullptr) && (selectedGameObject != root)) 
	{
		selectedGameObject->to_delete = true;
	}
	
	//ImGuizmo::OPERATION mCurrentGizmoOperation;


	/*ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE mode = ImGuizmo::WORLD;

	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(App->editor->w_pos.x + App->editor->tab.x, App->editor->w_pos.y + App->editor->tab.y, App->editor->image_size.x, App->editor->image_size.y);

	ImGuizmo::OPERATION mCurrentGizmoOperation;

	if ((App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN))
		mCurrentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

	else if ((App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN))
		mCurrentGizmoOperation = ImGuizmo::OPERATION::ROTATE;

	else if ((App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN))
		mCurrentGizmoOperation = ImGuizmo::OPERATION::SCALE;*/
	
	/*math::float4x4 matrix = global_matrix.Transposed();

	ImGuizmo::Manipulate(App->camera->editor_cam->GetViewMatrix(), App->camera->editor_cam->GetProjectionMatrix(), mCurrentGizmoOperation, ImGuizmo::WORLD);
		App->camera->current_camera->GetViewMatrix(), App->camera->current_camera->GetProjectionMatrix(), mCurrentGizmoOperation, ImGuizmo::WORLD, matrix.ptr()*/

	return UPDATE_CONTINUE;
}


void ModuleScene::CreateMainCamera()
{
	GameObject* main_cam = new GameObject(new Camera());
	AddGameObject(main_cam);
	main_cam->SetName("Main Camera");
	main_camera = (Camera*)main_cam->GetComponent(ComponentType::CAMERA);
	LOG("Cam created");
}



void ModuleScene::Play()
{
	Time::Start();
}
void ModuleScene::Stop()
{
	Time::Stop();
}

