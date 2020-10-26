#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <string>
#include <vector>
#include "Component.h"

/// ///////////////////////////////////////// ///
/// The first game object in scene, aka root, ///
/// must have root as name, and recieve		  ///
/// nullpointer as parent					  ///
/// /////////////////////////////////////////////


class GameObject
{
public:
	//Constructor
	GameObject(std::string& name, GameObject* parent, bool active = true);

	//Destructor
	~GameObject();


	//Change game object active state to enabled
	void Enable();

	//Change game object active state to disabled
	void Disable();

	//Update all its childs and components
	void Update();
	
	//Return its active state
	bool IsEnabled();


	//Get game object's parent
	GameObject* const GetParent() const;

	//Add another game object as a child
	void AddGameObjectAsChild(GameObject* game_object);

	//Change parent game object pointer
	void ChangeParent(GameObject* new_parent);

	//Get a pointer to this game object childs vector
	std::vector<GameObject*>* const GetChilds();

	//Erasing a child from this game object but not deleting it
	void EraseChildPointer(GameObject* child);

	//Completly delete a child. Send a true boolean to delete all childs!
	void DeleteChild(GameObject* child);

	//Send this game object childs to this parent
	void GetChildsNewParent();


	//Add a component to this game object. Enter type!
	void CreateComponent(Component::COMPONENT_TYPE type);

	//Check and add component
	void CheckAddComponent(Component* new_comp);

	//Get a pointer to this game object components vector
	std::vector<Component*>* const GetComponents();


	//Get a reference to this game object name
	std::string& const GetName();

	//Change game object's name. Passed by reference
	void ChangeName(std::string& new_name);

private:
	GameObject*					parent;
	bool						active;
	std::string					name;
	std::vector<Component*>		components;
	std::vector<GameObject*>	childs;
};

#endif // !__GAMEOBJECT_H__