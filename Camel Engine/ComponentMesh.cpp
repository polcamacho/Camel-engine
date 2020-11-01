#include "ComponentMesh.h"
#include "Component.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

ComponentMesh::ComponentMesh(GameObject* owner, bool active) : Component(COMPONENT_TYPE::MESH, owner, active)
{
	mesh = nullptr;
}

ComponentMesh::~ComponentMesh()
{
	mesh->clear();
}

void ComponentMesh::Update()
{
	App->renderer3D->RenderMesh(mesh);
}

void ComponentMesh::Enable()
{
	active = true;
}

void ComponentMesh::Disable()
{
	active = false;
}

bool ComponentMesh::IsEnabled()
{
	return active;
}

void ComponentMesh::AssignMesh(const char* path)
{
	mesh = App->scene_intro->AddMesh(path);
}
