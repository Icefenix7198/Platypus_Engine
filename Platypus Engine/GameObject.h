#pragma once

#include "Globals.h"

#include <vector>
#include <string>

enum ComponentType;

class Component;
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;

class GameObject
{
public:
	GameObject();
	GameObject(std::string name, bool active = true);
	GameObject(std::string name, GameObject* parent, bool active = true);
	~GameObject();

	void Update();
	Component* CreateComponent(ComponentType type);
	bool HasComponent(ComponentType type);
	int GetComponentPosition(Component* comp); //Return the position in the Component Vector, returns -1 if there is none
	

	ComponentTransform* objTransform = nullptr; //Due to the importance of the transform we have another reference to it

	bool active;
	std::string name;
	std::vector<Component*> components;
	GameObject* parent;
	std::vector<GameObject*> children;
private:

};
