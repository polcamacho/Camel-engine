#include "ComponentMesh.h"
#include "Component.h"

ComponentMesh::ComponentMesh(GameObject* owner, bool active) : Component(COMPONENT_TYPE::MESH, owner, active)
{
	requesting_mesh = false;
}

ComponentMesh::~ComponentMesh()
{

}

void ComponentMesh::Update()
{
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

std::vector<MeshPart*>* ComponentMesh::GetMesh()
{
	return mesh;
}