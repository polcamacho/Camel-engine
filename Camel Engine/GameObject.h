#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <string>
#include <vector>

class Component;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Enable();
	void Disable();
	void Update();
	
	bool IsEnabled();

	GameObject* GetParent();
	std::vector<GameObject*>* GetChilds();
	std::vector<Component*>* GetComponents();

private:
	GameObject*					parent;
	bool						active;
	std::string					name;
	std::vector<Component*>		components;
	std::vector<GameObject*>	childs;
};

#endif // !__GAMEOBJECT_H__