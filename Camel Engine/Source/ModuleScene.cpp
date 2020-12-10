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

