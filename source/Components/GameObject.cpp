#include "../../include/Components/GameObject.h"

GameObject::GameObject()
{
	transform = new Transform(this);
}

GameObject::~GameObject()
{
	//delete transform;
	while (children.size())
	{
		GameObject* obj = children.back();
		children.pop_back();
		delete obj;
	}
	while (components.size())
	{
		Component* obj = components.back();
		components.pop_back();
		delete obj;
	}
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
}

Component* GameObject::GetComponent(std::string type)
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end();)
	{
		if ((*it)->name == type)
		{
			return (*it);
		}
		else
		{
			++it;
		}
	}
	return nullptr;
}

void GameObject::RemoveComponent(std::string type)
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end();)
	{
		if ((*it)->name == type)
		{
			it = components.erase(it);
			break;
		}
		else
		{
			++it;
		}
	}

}
