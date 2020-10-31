#include "Component.h"

Component::Component(COMPONENT_TYPE type, GameObject* owner, bool active) : type(type), owner(owner), active(active) {}

Component::~Component()
{
	owner = nullptr;
}

void Component::Update()
{
	return;
}

void Component::Enable()
{
	active = true;
	return;
}

void Component::Disable()
{
	active = false;
	return;
}

bool Component::IsEnabled()
{
	return active;
}

Component::COMPONENT_TYPE Component::GetComponentType()
{
	return COMPONENT_TYPE();
}
