#include "..\..\include\Components\PlayerMovement.h"
#include "..\..\include\InputManager.h"
#include "..\..\include\UpdateManager.h"
#include "..\..\include\Components\GameObject.h"
#include "..\..\include\Components\Transform.h"
#include "pgr.h"

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
	if (IsKeyDown('a') || keysDown.left) moveDir.z -= 1; //A
	if (IsKeyDown('d') || keysDown.right)  moveDir.z += 1; //D
	if (IsKeyDown('s') || keysDown.down)  moveDir.x -= 1; //S
	if (IsKeyDown('w') || keysDown.up)  moveDir.x += 1; //W
	if (IsKeyDown('q'))  moveDir.y += 1; //Q
	if (IsKeyDown('e'))  moveDir.y -= 1; //E
	glm::normalize(moveDir);
	moveDir = glm::vec3(rotMat * glm::vec4(moveDir, 1.0));
	this->gameObject->transform->position += moveDir * moveSpeed;
}

PlayerMovement::~PlayerMovement()
{
	RemoveUpdatable(this);
}
