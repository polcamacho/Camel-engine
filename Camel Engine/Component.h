#pragma once
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class GameObject;

class Component {

public:
	
	//Type enum
	enum class COMPONENT_TYPE{
		TRANSFORM,
		MESH,
		MATERIAL,
		NONE
	};

public:

	//Constructor
	Component(COMPONENT_TYPE type, GameObject* owner, bool active = true);

	//Destructor
	~Component();


	//Update
	virtual void Update();

	//Enable component
	virtual void Enable();

	//Disable component
	virtual void Disable();

	//Check if component is enabled
	virtual bool IsEnabled();


	//Get component type
	COMPONENT_TYPE GetComponentType();

public:

	bool			active;

private:
	COMPONENT_TYPE	type;
	GameObject*		owner;
};

#endif // !__COMPONENT_H__