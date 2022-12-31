#include "..\..\include\Components\Transform.h"
#include "..\..\include\Components\GameObject.h"

Transform::Transform(GameObject* owner): Component(owner)
{
	name = "transform";
}

glm::mat4 Transform::GetLocalTranformMatrix()
{
	glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	R *= glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	R *= glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 modelMatrix = T * R * S;
	return modelMatrix;
}

glm::mat4 Transform::GetGlobalTransformMatrix()
{
	glm::mat4 localMatrix = this->GetLocalTranformMatrix();
	if (this->gameObject->parent != nullptr)
	{
		localMatrix = this->gameObject->parent->transform->GetGlobalTransformMatrix() * localMatrix;
	}
	return localMatrix;
}
