#pragma once
#include "Component.h"

class PlayerMovement : public Component
{
public:
	PlayerMovement(GameObject* owner);
	void Update(float deltaTime);
	~PlayerMovement();
};