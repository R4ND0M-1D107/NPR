#pragma once
#include "Component.h"
#include "Transform.h"
#include <vector>

class GameObject
{
private:
	std::vector<Component*> components;
public:
	GameObject();
	GameObject* gameObject;
	Transform* transform;
	GameObject* parent;
	std::vector <GameObject*> children;
	void AddComponent(Component* component);
	Component* GetComponent(std::string type);
	void RemoveComponent(std::string type);

};