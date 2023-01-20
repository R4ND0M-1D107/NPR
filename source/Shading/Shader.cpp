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
		this->lights[i].shadowCubemap = glGetUniformLocation(this->program, ("lights[" + idx + "].shadowCubemap").c_str());
		this->lights[i].shadowMap = glGetUniformLocation(this->program, ("lights[" + idx + "].shadowMap").c_str());
		this->lights[i].mat = glGetUniformLocation(this->program, ("lights[" + idx + "].lightSpaceMat").c_str());
		//printf("cubemap pos %d\n", this->lights[i].shadowCubemap);
	}
	this->viewerPos = glGetUniformLocation(this->program, "viewerPosition");
}

void Shader::DeserializeLocations(std::ifstream &file)
{
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
			printf("%s : %d\n", name.c_str(), location);
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

Shader::Shader(std::string txt, std::string geometryShader)
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
			pgr::createShaderFromFile(GL_GEOMETRY_SHADER, geometryShader),
			pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fs),
			0
			};
			if (shaders[0] == 0 || shaders[1] == 0 || shaders[2] == 0)
				pgr::dieWithError("Shader creation failed");

			program = pgr::createProgram(shaders);
			if (program == 0)
			{
				pgr::dieWithError("Shader creation failed1");
			}
		}
		DeserializeLocations(file);
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
		
		DeserializeLocations(file);
		
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

void Shader::SetLights(std::vector<Light*> lightComponents, int j)
{
	int i = 0;
	for (; i < _MaxLights && i < lightComponents.size(); i++)
	{
		glUniform3fv(lights[i].color, 1, glm::value_ptr(lightComponents[i]->color));
		glUniform3fv(lights[i].distribution, 1, glm::value_ptr(lightComponents[i]->distribution));
		glUniform4fv(lights[i].position, 1, glm::value_ptr(lightComponents[i]->getPosition()));
		glUniform3fv(lights[i].direction, 1, glm::value_ptr(lightComponents[i]->direction));
		glUniform1f(lights[i].cutoff, lightComponents[i]->cosCutOff);
		glUniform1f(lights[i].exponent, lightComponents[i]->exponent);
		glActiveTexture(GL_TEXTURE0+j);
		glBindTexture(GL_TEXTURE_CUBE_MAP, lightComponents[i]->shadowCubemap);
		glUniform1i(lights[i].shadowCubemap, j);
		j++;
		glActiveTexture(GL_TEXTURE0 + j);
		glBindTexture(GL_TEXTURE_2D, lightComponents[i]->shadowMap);
		glUniform1i(lights[i].shadowMap, j);
		j++;
		glUniformMatrix4fv(lights[i].mat, 1, GL_FALSE, glm::value_ptr(lightComponents[i]->mat));
	}
	for (; i < _MaxLights; i++)
	{
		glUniform3fv(lights[i].color, 1, glm::value_ptr(glm::vec3(0)));
		glUniform3fv(lights[i].distribution, 1, glm::value_ptr(glm::vec3(0)));
		glUniform4fv(lights[i].position, 1, glm::value_ptr(glm::vec4(0)));
		glUniform3fv(lights[i].direction, 1, glm::value_ptr(glm::vec3(0)));
		glUniform1f(lights[i].cutoff, 0);
		glUniform1f(lights[i].exponent, 0);
		glActiveTexture(GL_TEXTURE0+j);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glUniform1i(lights[i].shadowCubemap, j);
		j++;
		glActiveTexture(GL_TEXTURE0 + j);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(lights[i].shadowMap, j);
		j++;
		glUniformMatrix4fv(lights[i].mat, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
	}
}

void Shader::SetVec3(std::string location, glm::vec3 vector)
{
	glUniform3fv(this->locations[location], 1, glm::value_ptr(vector));
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
	}

	for (auto const& vec2 : material->vec2s)
	{
		glUniform2fv(this->locations[vec2.first], 1, glm::value_ptr(vec2.second));
	}

	if (lightsActive)
	{
		this->SetLights(Light::GetAllLights(), i);
		glUniform3fv(viewerPos, 1, glm::value_ptr(GetCameraPosition())); 
	}

	CHECK_GL_ERROR();
}
