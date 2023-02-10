#pragma once
#include "Component.h"
#include <pgr.h>

class Transform : Component
{
public:
	Transform(GameObject* owner);
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 GetLocalTranformMatrix();
	glm::mat4 GetGlobalTransformMatrix();
	void OnGUIDraw();
};