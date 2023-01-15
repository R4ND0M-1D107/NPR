#pragma once
#include "Light.h"
#include "pgr.h"

struct direction
{
	glm::vec3 front;
	glm::vec3 up;
};

class PointLight : public Light
{
private:
	struct direction
	{
		glm::vec3 front;
		glm::vec3 up;
	};
	direction directions[6] =
	{
		{glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
        {glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
        {glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)},
        {glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
        {glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)}
	};
public:
	PointLight(GameObject* owner, pugi::xml_node xmlNode);
	~PointLight();
	void ComputeShadows();
};