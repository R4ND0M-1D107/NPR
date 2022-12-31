#include "..\..\include\Components\Light.h"
#include "..\..\include\Components\GameObject.h"
#include "..\..\include\Components\Transform.h"


std::vector<Light*> Light::activeLights = std::vector<Light*>();

Light::Light(GameObject* owner, pugi::xml_node xmlNode) : Component(owner)
{
	activeLights.push_back(this);
	color = glm::vec3(xmlNode.child("color").attribute("r").as_float(), xmlNode.child("color").attribute("g").as_float(), xmlNode.child("color").attribute("b").as_float());
	distribution = glm::vec3(xmlNode.child("distribution").attribute("x").as_float(), xmlNode.child("distribution").attribute("y").as_float(), xmlNode.child("distribution").attribute("z").as_float());
	point = xmlNode.child("point").attribute("bool").as_float();
	direction = glm::vec3(xmlNode.child("direction").attribute("x").as_float(), xmlNode.child("direction").attribute("y").as_float(), xmlNode.child("direction").attribute("z").as_float());
	cosCutOff = xmlNode.child("cosCutOff").attribute("value").as_float();
	exponent = xmlNode.child("exponent").attribute("value").as_float();
}

Light::~Light()
{
	activeLights.erase(std::remove(activeLights.begin(), activeLights.end(), this), activeLights.end());
}

std::vector<Light*> Light::GetAllLights()
{
	return activeLights;
}

glm::vec4 Light::getPosition()
{
	return glm::vec4(this->gameObject->transform->position, point);
}