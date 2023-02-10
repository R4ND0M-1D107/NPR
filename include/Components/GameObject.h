#pragma once
#include "Component.h"
#include "Transform.h"
#include <vector>

class GameObject
{
private:
public:
	GameObject();
	~GameObject();
	std::vector<Component*> components;
	Transform* transform;
	GameObject* parent;
	std::vector <GameObject*> children;
	void AddComponent(Component* component);
	Component* GetComponent(std::string type);
	void RemoveComponent(std::string type);
	std::string name;
	bool active;
};