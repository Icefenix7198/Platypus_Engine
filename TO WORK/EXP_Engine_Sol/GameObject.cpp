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
	Component* ret = nullptr;
	
	switch (type)
	{
	case TRANSFORM:

		for (int i = 0; i < components.size(); i++)
		{
			if (components.at(i)->type == ComponentType::TRANSFORM) { nonTransform = false; }
		}
		if (nonTransform && objTransform ==nullptr)
		{
			LOG("Create Component Transform")				
			ret = new ComponentTransform;
			components.push_back(ret);
			objTransform = (ComponentTransform*)ret;
		}

		break;
	case MESH:

		//Create component mesh
		LOG("Create Component Mesh");
		ret = new ComponentMesh;
		components.push_back(ret);

		break;
	case MATERIAL:

		//Create component material
		LOG("Create Component Transform")
		ret = new ComponentTransform;
		components.push_back(ret);

		break;
	default:
		break;
	}
	
	return ret;
}


