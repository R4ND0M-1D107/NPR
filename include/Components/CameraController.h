#pragma once
#include "Component.h"

class CameraController : public Component
{
public:
	CameraController(GameObject* owner);
	~CameraController();
	void Update(float deltaTime);
};