#pragma once
#include "Component.h"
#include "pgr.h"
#include "../pugixml/pugixml.hpp"

class Light : public Component
{
protected:
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint shadowFBO;
	GLuint depthBuffer;
private:
	static std::vector<Light*> activeLights;
public:
	Light(GameObject* owner, pugi::xml_node xmlNode);
	~Light();
	glm::vec3 color;
	float point;
	glm::vec3 distribution;
	glm::vec4 getPosition();
	glm::vec3 direction; //zero vector if point
	float cosCutOff;
	float exponent; //spotExponent or attenuation
	GLuint shadowMap;
	static std::vector<Light*> GetAllLights();
	static void ClearLights();
	virtual void ComputeShadows();
};