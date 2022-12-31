#pragma once
#include "Component.h"
#include "../Shading/Material.h"
#include "../Shading/Shader.h"
#include "../Shading/Mesh.h"

class Renderer : public Component
{
public:
	Renderer(GameObject* owner, Shader* shader, Material* material, Mesh* mesh);
	~Renderer();
	Material* material;
	Shader* shader;
	Mesh* mesh;
	void Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
};