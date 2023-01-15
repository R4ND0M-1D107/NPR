#pragma once
#include "Component.h"
#include "../Shading/Shader.h"
#include "../Shading/Mesh.h"

class ShadowRenderer : public Component
{
public:
	ShadowRenderer(GameObject* owner, Shader* shader, Mesh* mesh);
	~ShadowRenderer();
	Shader* shader;
	Mesh* mesh;
	void Render(std::vector<glm::mat4> projViewMats, glm::vec3 lightPos);
};