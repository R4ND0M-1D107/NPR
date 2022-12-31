#include "..\..\include\Components\Camera.h"
#include "..\..\include\Components\GameObject.h"
#include "..\..\include\Components\Transform.h"
#include "..\..\include\RenderingManager.h"

Camera::Camera(GameObject* owner) : Component(owner)
{
	SetMainCamera(this);
}

glm::mat4 Camera::GetPerspectiveMatrix()
{
	float width = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
	float height = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));
	float aspectRatio = width / height;
	glm::mat4 perspectiveM = glm::perspective(glm::radians(55.0f), aspectRatio, 0.1f, 45.0f);
	return perspectiveM;
}

glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 right = normalize(cross(globalUp, direction));
glm::vec3 up = cross(direction, right);
glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);

glm::mat4 Camera::GetViewMatrix()
{
	glm::vec3 front;
	float yaw = this->gameObject->transform->rotation.x;
	float pitch = this->gameObject->transform->rotation.y;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//front = normalize(front);

	return glm::lookAt(this->gameObject->transform->position, this->gameObject->transform->position + front, up);
}
