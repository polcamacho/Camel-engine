#pragma once
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class GameObject;

class Component {
public:
	static enum class COMPONENT_TYPE{
		TRANSFORM,
		MESH,
		MATERIAL,
		NONE
	};

	Component(COMPONENT_TYPE type);
	~Component();

	virtual void Update();
	virtual void Enable();
	virtual void Disable();
	virtual void IsEnabled();

	void GetComponentType();

private:
	COMPONENT_TYPE	type;
	bool			active;
	GameObject*		owner;
};

#endif // !__COMPONENT_H__