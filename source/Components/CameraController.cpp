#include "..\..\include\Components\CameraController.h"
#include "..\..\include\Components\GameObject.h"
#include "..\..\include\Components\Transform.h"
#include "pgr.h"
#include "../../include/InputManager.h"
#include "../../include/UpdateManager.h"
#include "../../libs/imgui/imgui.h"

float yaw = 0;
float pitch = 0;
float sensitivity = 4;

CameraController::CameraController(GameObject* owner) : Component(owner)
{
	name = "cameraController";
	AddUpdatable(this);
	yaw = this->gameObject->transform->rotation.x;
	pitch = this->gameObject->transform->rotation.y;
}

CameraController::~CameraController()
{
	RemoveMouseListener(this);
}

void CameraController::Update(float deltaTime)
{
	ImVec2 mD = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
	glm::vec2 mouseDelta = glm::vec2(mD.x, mD.y);
	if (std::abs(mD.x) > glutGet(GLUT_WINDOW_WIDTH) * 0.85 || std::abs(mD.y) > glutGet(GLUT_WINDOW_HEIGHT) * 0.85)
	{
		mouseDelta = glm::vec2(0);
	}
	

	yaw -= sensitivity * mouseDelta.x * deltaTime;
	pitch += sensitivity * mouseDelta.y * deltaTime;

	pitch = (pitch > 89.0f) ? 89.0f : pitch; //why not 90 - gimball lock
	pitch = (pitch < -89.0f) ? -89.0f : pitch;

	this->gameObject->transform->rotation.x = yaw;
	this->gameObject->transform->rotation.y = pitch;

	if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
	{
		int height = glutGet(GLUT_WINDOW_HEIGHT);
		int width = glutGet(GLUT_WINDOW_WIDTH);
		float mouseX = ImGui::GetMousePos().x;//ImGui::GetCursorPosX();
		float mouseY = ImGui::GetMousePos().y;
		float delta = 10;
		if (mouseX < delta) 
		{
			glutWarpPointer(width - delta, mouseY);
		}
		else if (mouseX > width - delta)
		{
			glutWarpPointer(delta, mouseY);
		}
		
		if (mouseY < delta) {
			glutWarpPointer(mouseX, height - delta);
		}
		else if (mouseY > height - delta)
		{
			glutWarpPointer(mouseX, delta);
		}
	}
	
	ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
}

void CameraController::OnGUIDraw()
{
	ImGui::DragFloat("Sensitivity", &sensitivity);
}
