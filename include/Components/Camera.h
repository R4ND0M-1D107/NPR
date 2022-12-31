#pragma once
#include "Component.h"
#include "pgr.h"

class Camera : public Component
{
public:
	Camera(GameObject* owner);
	glm::mat4 GetPerspectiveMatrix();
	glm::mat4 GetViewMatrix();
	~Camera();
};