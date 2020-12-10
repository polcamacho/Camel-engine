#include "Component.h"

Component::Component() : enabled(true), gameObject(nullptr) {
   
    type = ComponentType::NO_TYPE;
}

Component::~Component()
{
    gameObject = nullptr;
}

void Component::Update(){}

void Component::Enable() { enabled = true; }

void Component::Disable() { enabled = false; }

bool Component::IsEnabled()
{
    return enabled;
}

ComponentType Component::GetType()
{
    return type;
}

void Component::SetGameObject(GameObject* g_gameObject)
{
    gameObject = g_gameObject;
}

GameObject* Component::GetGameObject()
{
    return gameObject;
}
