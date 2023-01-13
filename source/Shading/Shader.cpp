#include "..\..\include\Shading\Shader.h"
#include <fstream>
#include <string>
#include "Shader.h"
#include "..\..\include\DeferredRendering.h"

void Shader::SetupLights()
{
	this->lightsActive = true;
	for (int i = 0; i < _MaxLights; i++)
	{
		std::string idx = std::to_string(i);
		this->lights[i].color = glGetUniformLocation(this->program, ("lights[" + idx + "].color").c_str());
		this->lights[i].distribution = glGetUniformLocation(this->program, ("lights[" + idx + "].distribution").c_str());
		this->lights[i].position = glGetUniformLocation(this->program, ("lights[" + idx + "].position").c_str());
		this->lights[i].direction = glGetUniformLocation(this->program, ("lights[" + idx + "].direction").c_str());
		this->lights[i].cutoff = glGetUniformLocation(this->program, ("lights[" + idx + "].cutoff").c_str());
		this->lights[i].exponent = glGetUniformLocation(this->program, ("lights[" + idx + "].exponent").c_str());
		this->viewerPos = glGetUniformLocation(this->program, "viewerPosition");
	}
}

Shader::Shader(std::string txt)
{
	std::ifstream file(txt, std::ios::in);
	if (!file)
	{
		pgr::dieWithError("shader file doesn't exist");
	}
	else
	{
		std::string vs;
		std::string fs;
		if (!(file >> vs) || !(file >> fs))
		{
			pgr::dieWithError("Invalid shader definition");
		}
		else
		{
			GLuint shaders[] = {
			pgr::createShaderFromFile(GL_VERTEX_SHADER, vs),
			pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fs),
			0,
			};
			if (shaders[0] == 0 || shaders[1] == 0)
				pgr::dieWithError("Shader creation failed");

			program = pgr::createProgram(shaders);
			if (program == 0)
			{
				pgr::dieWithError("Shader creation failed");
			}
		}
		

		std::string type;
		std::string name;
		GLint location;
		while (file >> type >> location >> name)
		{
			if (type == "out")
			{

				//glBindFragDataLocation(program, location, name.c_str());
				CHECK_GL_ERROR();
			}
			else if (type == "attribute")
			{

				glBindAttribLocation(program, location, name.c_str());
				CHECK_GL_ERROR();
			}
			else if (type == "uniform")
			{
				location = glGetUniformLocation(program, name.c_str());
				CHECK_GL_ERROR();
			}
			else if (type == "lights")
			{
				SetupLights();
			}
			else
			{
				pgr::dieWithError("Invalid shader definition");
			}
			//printf("%s %d %s\n", txt.c_str(), location, name.c_str());
			locations.insert(std::pair<std::string, GLint>(name, location));
		}
	}
}

void Shader::SetMatrices(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::mat4 modelMatrix)
{
	//TODO add ifs in case shader doesn't need matrices
	glUniformMatrix4fv(locations["projectionMatrix"], 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(locations["viewMatrix"], 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(locations["modelMatrix"], 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

Shader::~Shader()
{
}

void Shader::SetLights(std::vector<Light*> lightComponents)
{
	for (int i = 0; i < lightComponents.size() && i < _MaxLights; i++)
	{
		glUniform3fv(lights[i].color, 1, glm::value_ptr(lightComponents[i]->color));
		glUniform3fv(lights[i].distribution, 1, glm::value_ptr(lightComponents[i]->distribution));
		glUniform4fv(lights[i].position, 1, glm::value_ptr(lightComponents[i]->getPosition()));
		glUniform3fv(lights[i].direction, 1, glm::value_ptr(lightComponents[i]->direction));
		glUniform1f(lights[i].cutoff, lightComponents[i]->cosCutOff);
		glUniform1f(lights[i].exponent, lightComponents[i]->exponent);
	}
}

void Shader::UseMaterial(Material* material)
{
	int i = 0;
	for (auto const& tex : material->textures)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, tex.second);
		glUniform1i(this->locations[tex.first], i);
		i++;
	}

	for (auto const& floatNum : material->floats)
	{
		glUniform1f(this->locations[floatNum.first], floatNum.second);
		i++;
	}

	for (auto const& vec2 : material->vec2s)
	{
		glUniform2fv(this->locations[vec2.first], 1, glm::value_ptr(vec2.second));
		i++;
	}

	if (lightsActive)
	{
		this->SetLights(Light::GetAllLights());
		glUniform3fv(viewerPos, 1, glm::value_ptr(GetCameraPosition())); 
	}
}
