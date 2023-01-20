#pragma once
#include "Light.h"
#include "pgr.h"

class SpotLight : public Light
{
public:
	SpotLight(GameObject* owner, pugi::xml_node xmlNode);
	~SpotLight();
	void ComputeShadows();
};