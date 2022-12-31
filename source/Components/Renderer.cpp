#include "..\..\include\Components\Renderer.h"
#include "..\..\include\Components\GameObject.h"
#include "..\..\include\RenderingManager.h"

Renderer::Renderer(GameObject* owner, Shader* _shader, Material* _material, Mesh* _mesh) : Component(owner)
{
	shader = _shader;
	material = _material;
	mesh = _mesh;
	mesh->setup(shader);
	name = "renderer";
	AddRenderer(this);
}

Renderer::~Renderer()
{
	RemoveRenderer(this);
}

void Renderer::Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glUseProgram(shader->program);

	shader->SetMatrices(viewMatrix, projectionMatrix, this->gameObject->transform->GetGlobalTransformMatrix());
	shader->UseMaterial(material);



	glBindVertexArray(mesh->vertexArrayObject);
	glDrawElements(GL_TRIANGLES, mesh->numTriangles * 3, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
	CHECK_GL_ERROR();
}
