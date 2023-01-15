#pragma once
#include <map>
#include <pgr.h>
#include <string.h>
#include "Material.h"
#include "../Components/Light.h"

const int _MaxLights = 4;

class Shader
{
private:
	bool lightsActive;
	void SetupLights();
	struct
	{
		GLuint color;
		GLuint distribution;
		GLuint position;
		GLuint direction;
		GLuint cutoff;
		GLuint exponent;
		GLuint shadowCubemap;
	} lights[_MaxLights];
	GLuint viewerPos;
	void SetLights(std::vector<Light*> lightComponents, int j);
	void DeserializeLocations(std::ifstream &file);
public:
	Shader(std::string txt);
	Shader(std::string txt, std::string geometryShader);
	void SetMatrices(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::mat4 modelMatrix);
	void SetVec3(std::string location, glm::vec3 vector);
	~Shader();
	GLuint program;
	std::map<std::string, GLint> locations;
    void UseMaterial(Material *material);
};