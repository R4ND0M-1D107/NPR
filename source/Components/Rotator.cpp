#include "..\..\include\Components\Rotator.h"
#include "..\..\include\Components\GameObject.h"
#include "..\..\include\Components\Transform.h"
#include "pgr.h"
#include "../../include/UpdateManager.h"


Rotator::Rotator(GameObject* owner) : Component(owner)
{
	name = "rotator";
	AddUpdatable(this);
}

Rotator::~Rotator()
{
	RemoveUpdatable(this);
}

void Rotator::Update(float deltaTime)
{
	this->gameObject->transform->rotation.y += 5*deltaTime;
}
