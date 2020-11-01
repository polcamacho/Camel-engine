#pragma once
#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentTransform : public Component 
{
public:

	// Parameter Constructor
	ComponentTransform(GameObject* owner, math::float3 position = math::float3::zero, math::float3 rotation = math::float3::zero, math::float3 scale = math::float3::one);

	// Copy Constructor
	ComponentTransform(ComponentTransform& transform);

	// Destructor
	~ComponentTransform();


	// Get this transform position
	math::float3 GetPosition() const;

	// Get this transform rotation
	math::float3 GetRotation() const;

	// Get this transform scale
	math::float3 GetScale() const;

	// Get this transform quaternion
	math::Quat GetQuaternion() const;

	// Get transform matrix
	math::float4x4 GetTransformMatrix() const;

	
	// Update global matrix
	void Update();


	// Set this transform position
	void SetPosition(math::float3& new_pos);

	// Set this transform rotation
	void SetRotation(math::float3& new_rotation);

	// Set this transform scale
	void SetScale(math::float3& new_scale);

	// Set this transform quaternion
	void SetQuaternion(math::Quat& quaternion);

	
private:

	math::float3 position;
	math::float3 euler_angles;
	math::float3 scale;
	math::Quat quaternion_rotation;

	math::float4x4 local_matrix;
	math::float4x4 global_matrix;
};

#endif // !__COMPONENTTRANSFORM_H__