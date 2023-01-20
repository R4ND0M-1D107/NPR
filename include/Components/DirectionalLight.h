#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(GameObject* owner, pugi::xml_node xmlNode);
	~DirectionalLight();
	void ComputeShadows();
};