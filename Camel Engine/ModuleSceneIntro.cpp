#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ComponentMesh.h"
#include "Primitive.h"
#include <gl/GL.h>
#include "glew/include/glew.h"
#include <gl/GLU.h>

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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
	a = AddMesh("Assets/BakerHouse.fbx");
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
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	App->renderer3D->RenderMesh(a);

	return UPDATE_CONTINUE;
}

std::vector<MeshPart*>* ModuleSceneIntro::AddMesh(const char* path)
{
	std::vector<FullMesh*>::iterator it = meshes.begin();
	for (; it < meshes.end() && meshes.size() > 0; ++it)
	{
		if (path == (*it)->id) return &(*it)->parts;
	}
	return App->load_object->LoadObjectData(path);
}