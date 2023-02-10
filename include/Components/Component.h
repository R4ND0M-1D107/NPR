#pragma once
#include <string>

class GameObject;

class Component
{
public:
	Component(GameObject* owner);
	virtual ~Component();
	GameObject* gameObject;
	std::string name = "undefined";
	bool enabled = true;
	virtual void Update(float deltaTime);
	virtual void OnGUIDraw();
};