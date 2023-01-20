#pragma once
#include "Component.h"

class Rotator : public Component
{
public:
	Rotator(GameObject* owner);
	~Rotator();
	void Update(float deltaTime);
};