#include "..\..\include\Components\ShadowRenderer.h"
#include "..\..\include\Components\GameObject.h"
#include "..\..\include\RenderingManager.h"
#include <string>

ShadowRenderer::ShadowRenderer(GameObject* owner, Shader* _pointShader, Shader* _dirShader, Mesh* _mesh) : Component(owner)
{
	pointShader = _pointShader;
	dirShader = _dirShader;
	mesh = _mesh;
	mesh->setup(pointShader);
	name = "renderer";
	AddShadowRenderer(this);
}

ShadowRenderer::~ShadowRenderer()
{
	RemoveShadowRenderer(this);
}

void ShadowRenderer::Render(std::vector<glm::mat4> projViewMats, glm::vec3 lightPos)
{
	glUseProgram(pointShader->program);

	pointShader->SetMatrices(glm::mat4(1), glm::mat4(1), this->gameObject->transform->GetGlobalTransformMatrix());
	for (int i = 0; i < 6; i++)
	{
		std::string idx = std::to_string(i);
		GLuint location = glGetUniformLocation(pointShader->program, ("shadowMatrices[" + idx + "]").c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projViewMats[i]));
	}
            
	pointShader->SetVec3("lightPos", lightPos);

	glBindVertexArray(mesh->vertexArrayObject);
	glDrawElements(GL_TRIANGLES, mesh->numTriangles * 3, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
	CHECK_GL_ERROR();
}

void ShadowRenderer::Render(glm::mat4 projMat, glm::mat4 viewMat, glm::vec3 lightPos)
{
	glUseProgram(dirShader->program);

	dirShader->SetMatrices(viewMat, projMat, this->gameObject->transform->GetGlobalTransformMatrix());
	dirShader->SetVec3("lightPos", lightPos);

	glBindVertexArray(mesh->vertexArrayObject);
	glDrawElements(GL_TRIANGLES, mesh->numTriangles * 3, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
	CHECK_GL_ERROR();
}
