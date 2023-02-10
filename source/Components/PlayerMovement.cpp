#include "..\..\include\Components\PlayerMovement.h"
#include "..\..\include\InputManager.h"
#include "..\..\include\UpdateManager.h"
#include "..\..\include\Components\GameObject.h"
#include "..\..\include\Components\Transform.h"
#include "pgr.h"
#include "../../libs/imgui/imgui.h"

PlayerMovement::PlayerMovement(GameObject* owner) : Component(owner)
{
	name = "playerMovement";
	AddUpdatable(this);
}

float moveSpeed = 0.05f;

void PlayerMovement::Update(float deltaTime)
{
	glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(this->gameObject->transform->rotation.x), glm::vec3(0.0f, -1.0f, 0.0f));
	glm::vec3 moveDir = glm::vec3(0, 0, 0);
	Keys keysDown = AreSpecialKeyDown();
	if (ImGui::IsKeyDown(ImGuiKey_A)) moveDir.z -= 1; //A
	if (ImGui::IsKeyDown(ImGuiKey_D))  moveDir.z += 1; //D
	if (ImGui::IsKeyDown(ImGuiKey_S))  moveDir.x -= 1; //S
	if (ImGui::IsKeyDown(ImGuiKey_W))  moveDir.x += 1; //W
	if (ImGui::IsKeyDown(ImGuiKey_Q))  moveDir.y += 1; //Q
	if (ImGui::IsKeyDown(ImGuiKey_E))  moveDir.y -= 1; //E
	glm::normalize(moveDir);
	moveDir = glm::vec3(rotMat * glm::vec4(moveDir, 1.0));
	this->gameObject->transform->position += moveDir * moveSpeed;
}

PlayerMovement::~PlayerMovement()
{
	RemoveUpdatable(this);
}

void PlayerMovement::OnGUIDraw()
{
	ImGui::DragFloat("Speed", &moveSpeed);
}
