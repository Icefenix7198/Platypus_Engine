#include "GameObject.h"
#include "Component.h"

#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

GameObject::GameObject()
{
	CreateComponent(ComponentType::TRANSFORM);
	name = "GameObject";
	active = true;
}

GameObject::GameObject(std::string _name, bool _active)
{
	CreateComponent(ComponentType::TRANSFORM);
	name = _name;
	active = _active;
}

GameObject::~GameObject()
{
	for (auto it = children.rbegin(); it != children.rend(); it++)
	{
		delete (*it);
		(*it) = nullptr;
	}
	for (auto it = components.rbegin(); it != components.rend(); it++)
	{
		delete (*it);
		(*it) = nullptr;
	}
}

void GameObject::Update()
{

}

Component* GameObject::CreateComponent(ComponentType type)
{
	bool nonTransform = true; //Only one transform can exist at a time
	switch (type)
	{
	case TRANSFORM:

		for (int i = 0; i < components.size(); i++)
		{
			if (components.at(i)->type == ComponentType::TRANSFORM) { nonTransform = false; }
		}
		if (nonTransform)
		{
			LOG("Create Component Transform")				
			ComponentTransform* temp = new ComponentTransform;
			components.push_back(temp);
		}

		break;
	case MESH:

		//Create component mesh
		//components.push_back();

		break;
	case MATERIAL:

		//Create component material
		//components.push_back();

		break;
	default:
		break;
	}
	return nullptr;
}


