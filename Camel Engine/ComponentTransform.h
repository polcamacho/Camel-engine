#pragma once
#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "glmath.h"

class ComponentTransform : public Component 
{
public:

	// Parameter Constructor
	ComponentTransform(GameObject* owner);

	// Destructor
	~ComponentTransform();


	// Get this transform position
	vec3 GetPosition() const;

	// Get this transform rotation
	vec3 GetRotation() const;

	// Get this transform scale
	vec3 GetScale() const;

	// Get transform matrix
	mat4x4 GetTransformMatrix() const;

	
	// Update global matrix
	void Update();


	// Set this transform position
	void SetPosition(vec3& new_pos);

	// Set this transform rotation
	void SetRotation(vec3& new_rotation);

	// Set this transform scale
	void SetScale(vec3& new_scale);

	
private:

	vec3 position;
	vec3 euler_angles;
	vec3 scale;

	mat4x4 local_matrix;
	mat4x4 global_matrix;
};

#endif // !__COMPONENTTRANSFORM_H__