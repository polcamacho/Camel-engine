#include "GameObject.h"

GameObject::GameObject(std::string& name, GameObject* parent, bool active) :name(name), parent(parent), active(active) {}

GameObject::~GameObject()
{
	//Empty components
	std::vector<Component*>::iterator comp = components.begin();
	for (; comp != components.end();++comp)delete* comp;
	components.clear();

	//Empty childs
	if (childs.size() != 0)
		GetChildsNewParent();

	//Deletes gameobject pointer from its parent
	parent->EraseChildPointer(this);
}

void GameObject::Enable()
{
	active = true;
}

void GameObject::Disable()
{
	active = false;
}

void GameObject::Update()
{
	//Update components
	std::vector<Component*>::iterator comp = components.begin();

	for (; comp != components.end(); ++comp)(*comp)->Update();

	//Update childs
	std::vector<GameObject*>::iterator g_o = childs.begin();

	for (; g_o != childs.end();++g_o)(*g_o)->Update();
}

bool GameObject::IsEnabled()
{
	return active;
}

GameObject* const GameObject::GetParent() const
{
	return parent;
}

void GameObject::AddGameObjectAsChild(GameObject* game_object)
{
	game_object->parent->EraseChildPointer(game_object);
	game_object->ChangeParent(this);
	childs.push_back(game_object);
}

void GameObject::ChangeParent(GameObject* new_parent)
{
	parent = new_parent;
}

std::vector<GameObject*>* const GameObject::GetChilds() 
{
	return &childs;
}

void GameObject::EraseChildPointer(GameObject* child)
{
	std::vector<GameObject*>::iterator it;
	if (child != nullptr) {
		for (it = childs.begin(); it != childs.end(); ++it) {
			if (*it == child)
			{
				childs.erase(it);
				return;
			}
		}
	}
}

void GameObject::DeleteChild(GameObject* child)
{
	std::vector<GameObject*>::iterator it;
	if (child != nullptr) {
		for (it = childs.begin(); it != childs.end(); ++it) {
			if (*it == child)
			{
				(*it)->GetChildsNewParent();
				childs.erase(it);
				delete* it;
				return;
			}
		}
	}
}

void GameObject::AddComponent(Component::COMPONENT_TYPE type)
{
	components.push_back(new Component(type));
}

std::vector<Component*>* const GameObject::GetComponents()
{
	return &components;
}

std::string& const GameObject::GetName()
{
	return name;
}

void GameObject::ChangeName(std::string& new_name)
{
	name = new_name;
}

void GameObject::GetChildsNewParent()
{
	GameObject* prnt;
	if (parent != nullptr)
		prnt = parent;
	else 
	{	
		prnt = parent->GetParent();
		while (prnt == nullptr && prnt->name != std::string("root"))
		{
			prnt = prnt->GetParent();
		}
	}
	std::vector<GameObject*>::iterator it = childs.begin();
	for (; it != childs.end(); ++it)
	{
		prnt->AddGameObjectAsChild((*it));
	}
	childs.clear();
}