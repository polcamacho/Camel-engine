#pragma once
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_
#include <vector>
#include <string>
#include "MathGeoLib/include/MathGeoLib.h"

class Component;
class Transform;
class GnMesh;
class Camera;
enum class ComponentType;

class GameObject {
public:
	GameObject();
	GameObject(GnMesh* mesh);
	GameObject(Camera* cam);
	~GameObject();

	void Update();
	void OnEditor();

	Component* GetComponent(ComponentType component);
	Component* AddComponent(ComponentType type);
	void AddComponent(Component* component);
	bool RemoveComponent(Component* component);

	const char* GetName();
	void SetName(const char* name);
	void SetTransform(Transform transform);
	Transform* GetTransform();

	GameObject* GetParent();
	void SetParent(GameObject* parent);

	void AddChild(GameObject* child);
	int GetChildAmount();
	GameObject* GetChildAt(int index);
	bool RemoveChild(GameObject* gameObject);
	void DeleteChildren();
	void UpdateChildrenTransforms();

	math::AABB GetAABB() const;
	void GenerateOBB();
	void DrawBoundingBox();

public:
	bool to_delete;
	AABB new_aabb;
	OBB obb;
	bool enabled;
	
private:
	bool bbox_enabled;
	std::string name;
	GameObject* parent;
	Transform* transform;
	std::vector<Component*> components;
	std::vector<GameObject*> children;
};

#endif // !_GAMEOBJECT_H