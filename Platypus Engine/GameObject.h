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

	//UUID 
	uint32_t GetUUID() { return UUID; };
	void SetUUID(uint32_t ID) { UUID = ID; };

	void Update();
	Component* CreateComponent(ComponentType type);
	bool HasComponent(ComponentType type);
	int GetComponentPosition(Component* comp); //Return the position in the Component Vector, returns -1 if there is none
	//Return the component if found in the vec, by default is the first one but could be the second, third, and if the num provided is negative starts by the end.
	Component* GetComponentByType(ComponentType type, int num = 0); 
	int GetNumComponentOfAKind(ComponentType type);

	ComponentTransform* objTransform = nullptr; //Due to the importance of the transform we have another reference to it

	bool active;
	std::string name;
	std::vector<Component*> components;
	GameObject* parent;
	std::vector<GameObject*> children;
private:
	uint32_t UUID;
};
