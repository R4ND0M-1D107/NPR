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
	} lights[_MaxLights];
	GLuint viewerPos;
	void SetLights(std::vector<Light*> lights);
public:
	Shader(std::string txt);
	void SetMatrices(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::mat4 modelMatrix);
	~Shader();
	GLuint program;
	std::map<std::string, GLint> locations;
    void UseMaterial(Material *material);
};