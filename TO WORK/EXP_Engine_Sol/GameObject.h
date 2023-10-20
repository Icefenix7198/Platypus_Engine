#pragma once

#include "Globals.h"

#include <vector>
#include <string>

enum ComponentType;

class Component;

class GameObject
{
public:
	GameObject();
	GameObject(std::string name, bool active = true);
	~GameObject();

	void Update();
	Component* CreateComponent(ComponentType type);

	bool active;
	std::string name;
	std::vector<Component*> components;
	GameObject* parent;
	std::vector<GameObject*> children;
private:

};
