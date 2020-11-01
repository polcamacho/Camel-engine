#include "ComponentTransform.h"
#include "GameObject.h"

ComponentTransform::ComponentTransform(GameObject* owner) :Component(Component::COMPONENT_TYPE::TRANSFORM, owner)
{
	// Setting value
	this->position = vec3(0.0f,0.0f,0.0f);
	this->euler_angles = vec3(0.0f, 0.0f, 0.0f);
	this->scale = vec3(1.0f, 1.0f, 1.0f);

	// First update of the transform matrix
	Update();
}

ComponentTransform::~ComponentTransform()
{
}

vec3 ComponentTransform::GetPosition() const
{
	return position;
}

vec3 ComponentTransform::GetRotation() const
{
	return euler_angles;
}

vec3 ComponentTransform::GetScale() const
{
	return scale;
}

mat4x4 ComponentTransform::GetTransformMatrix() const
{
	return global_matrix;
}

void ComponentTransform::Update()
{
	
}

void ComponentTransform::SetPosition(vec3& new_pos)
{
	position = new_pos;
}

void ComponentTransform::SetRotation(vec3& new_rotation)
{
}

void ComponentTransform::SetScale(vec3& new_scale)
{
	scale = new_scale;
}