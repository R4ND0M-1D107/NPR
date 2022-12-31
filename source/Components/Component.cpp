#include "..\..\include\Components\Component.h"
#include "..\..\include\Components\GameObject.h"

Component::Component(GameObject* owner)
{
	this->gameObject = owner;
	owner->AddComponent(this);
}

void Component::Update(float deltaTime)
{
}
