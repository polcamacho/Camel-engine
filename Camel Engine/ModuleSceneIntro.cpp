#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "Primitive.h"
#include <gl/GL.h>
#include "glew/include/glew.h"
#include <gl/GLU.h>

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
	c = nullptr;
	s = nullptr;
	pyr = nullptr;
	cyl = nullptr;
	root = nullptr;
	selected_game_obj = nullptr;
}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	//a = AddMesh("Assets/BakerHouse.fbx");
	
	
	//Testing
	root = new GameObject(std::string("root"), nullptr);
	/*root->AddGameObjectAsChild(new GameObject(std::string("test"), root));
	std::vector<GameObject*>::iterator it = root->GetChilds()->begin();
	for (; it != root->GetChilds()->end(); ++it)
	{
		if ((*it)->GetName() == std::string("test"))
		{
			(*it)->CreateComponent(Component::COMPONENT_TYPE::MESH);
			(*it)->GetComponentMesh()->AssignMesh("Assets/Models/warrior.fbx");
			CreateCheckersImage();
			return ret;
		}
	}*/
	App->load_object->CreateCheckersImage();
	//App->load_object->LoadTexture("Assets/Textures/Lenna.png");
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	std::vector<FullMesh*>::iterator im = meshes.begin();
	for (; im != meshes.end(); ++im) {
		std::vector<MeshPart*>::iterator ip = (*im)->parts.begin();
		for (; ip != (*im)->parts.end(); ++ip)delete* ip;

		(*im)->parts.clear();
	}
	meshes.clear();

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	PlaneP p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	root->Update();

	return UPDATE_CONTINUE;
}

// Add mesh to meshes vector. If it already exists, send pointer, not adding it
std::vector<MeshPart*>* ModuleSceneIntro::AddMesh(const char* path)
{
	
		std::string newid = path;
		newid.erase(0, newid.find_last_of("\\") +1);

	std::vector<FullMesh*>::iterator it = meshes.begin();
	for (; it < meshes.end() && meshes.size() > 0; ++it)
	{

		if (newid == (*it)->id) return &(*it)->parts;


	}
	return App->load_object->LoadObjectData(path);
}



void ModuleSceneIntro::CreateGameObjectByDragging(const char* path)
{
	std::string name = path;
	name.erase(0, name.find_last_of("\\") +1);
	
	root->AddGameObjectAsChild(new GameObject(name, root));
	std::vector<GameObject*>::iterator it = root->GetChilds()->begin();
	//ComponentTransform* trans = nullptr;
	for (; it != root->GetChilds()->end(); ++it)
	{
		if ((*it)->GetName() == name)
		{
			(*it)->CreateComponent(Component::COMPONENT_TYPE::MESH);
			(*it)->CreateComponent(Component::COMPONENT_TYPE::TRANSFORM);
			(*it)->GetComponentMesh()->AssignMesh(path);
		}
	}
}


void ModuleSceneIntro::UpdateGameObject(GameObject* parent)
{
	parent->Update();
	std::vector<GameObject*>::iterator iter = parent->childs.begin();
	for (iter; iter != parent->childs.end(); ++iter)
	{

		if (!(*iter)->show)
			UpdateGameObject((*iter));
	}
}

void ModuleSceneIntro::SetTextureDragging(uint& tex_id)
{
	std::vector<FullMesh*>::iterator im = meshes.begin();
	for (; im != meshes.end(); ++im) {
		std::vector<MeshPart*>::iterator ip = (*im)->parts.begin();
		for (; ip != (*im)->parts.end(); ++ip) {
			(*ip)->texture_id = tex_id;
		}


	}
}
