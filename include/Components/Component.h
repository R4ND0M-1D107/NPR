#pragma once
#include <string>

class GameObject;

class Component
{
public:
	Component(GameObject* owner);
	GameObject* gameObject;
	std::string name = "undefined";
	virtual void Update(float deltaTime);
};