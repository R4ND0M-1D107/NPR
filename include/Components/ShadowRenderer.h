#pragma once
#include "Component.h"
#include "../Shading/Shader.h"
#include "../Shading/Mesh.h"

class ShadowRenderer : public Component
{
public:
	ShadowRenderer(GameObject* owner, Shader* _pointShader, Shader* _dirShader, Mesh* _mesh);
	~ShadowRenderer();
	Shader* pointShader;
	Shader* dirShader;
	Mesh* mesh;
	void Render(std::vector<glm::mat4> projViewMats, glm::vec3 lightPos);
	void Render(glm::mat4 projMat, glm::mat4 viewMat, glm::vec3 lightPos);
};