#include "..\..\include\Components\CameraController.h"
#include "..\..\include\Components\GameObject.h"
#include "..\..\include\Components\Transform.h"
#include "pgr.h"
#include "../../include/InputManager.h"
#include "../../include/UpdateManager.h"

float yaw = 0;
float pitch = 0;
float sensitivity = 10;

CameraController::CameraController(GameObject* owner) : Component(owner)
{
	AddMouseListener(this);
	yaw = this->gameObject->transform->rotation.x;
	pitch = this->gameObject->transform->rotation.y;
}

void CameraController::Update(float deltaTime)
{
	glm::vec2 mouseDelta = GetMouseDelta();

	yaw -= sensitivity * mouseDelta.x * deltaTime;
	pitch += sensitivity * mouseDelta.y * deltaTime;

	pitch = (pitch > 89.0f) ? 89.0f : pitch; //why not 90 - gimball lock
	pitch = (pitch < -89.0f) ? -89.0f : pitch;

	this->gameObject->transform->rotation.x = yaw;
	this->gameObject->transform->rotation.y = pitch;
}
