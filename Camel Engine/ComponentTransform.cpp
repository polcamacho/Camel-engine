#include "ComponentTransform.h"
#include "GameObject.h"

ComponentTransform::ComponentTransform(GameObject* owner, math::float3 position, math::float3 rotation, math::float3 scale) :Component(Component::COMPONENT_TYPE::TRANSFORM, owner)
{
	// Setting value
	this->position = position;
	this->euler_angles = rotation;
	this->scale = scale;
	this->quaternion_rotation = math::Quat::FromEulerXYZ(math::DegToRad(euler_angles.x), math::DegToRad(euler_angles.y), math::DegToRad(euler_angles.z));
	global_matrix = global_matrix.identity;

	// First update of the transform matrix
	Update();
}

ComponentTransform::ComponentTransform(ComponentTransform& transform) : Component(COMPONENT_TYPE::TRANSFORM, transform.GetOwner())
{
	this->euler_angles = transform.euler_angles;
	this->position = transform.position;
	this->scale = transform.scale;
	this->quaternion_rotation = math::Quat::FromEulerXYZ(math::DegToRad(euler_angles.x), math::DegToRad(euler_angles.y), math::DegToRad(euler_angles.z));

	Update();
}

ComponentTransform::~ComponentTransform()
{
}

math::float3 ComponentTransform::GetPosition() const
{
	return position;
}

math::float3 ComponentTransform::GetRotation() const
{
	return euler_angles;
}

math::float3 ComponentTransform::GetScale() const
{
	return scale;
}

math::Quat ComponentTransform::GetQuaternion() const
{
	return quaternion_rotation;
}

math::float4x4 ComponentTransform::GetTransformMatrix() const
{
	return global_matrix;
}

void ComponentTransform::Update()
{
	math::float4x4 last_global = global_matrix;

	local_matrix = math::float4x4::FromTRS(this->position, this->quaternion_rotation, this->scale);

	GameObject* component_owner = GetOwner();

	if (component_owner != nullptr && component_owner->GetParent() != nullptr) {
		ComponentTransform* parent_transform = (ComponentTransform*)component_owner->GetParent()->GetComponent(Component::COMPONENT_TYPE::TRANSFORM);

		if (parent_transform != nullptr)
		{
			math::float4x4 aux = parent_transform->GetTransformMatrix();

			global_matrix = aux * local_matrix;
		}
		else
			global_matrix = local_matrix;

	}
	else
		global_matrix = local_matrix;
}

void ComponentTransform::SetPosition(math::float3& new_pos)
{
	position = new_pos;
}

void ComponentTransform::SetRotation(math::float3& new_rotation)
{
	euler_angles = new_rotation;
	quaternion_rotation = math::Quat::FromEulerXYZ(math::DegToRad(euler_angles.x), math::DegToRad(euler_angles.y), math::DegToRad(euler_angles.z));
}

void ComponentTransform::SetScale(math::float3& new_scale)
{
	scale = new_scale;
}

void ComponentTransform::SetQuaternion(math::Quat& quaternion)
{
	quaternion_rotation = quaternion;
}
