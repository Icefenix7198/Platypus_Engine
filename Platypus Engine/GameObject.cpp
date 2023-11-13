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
	components = {};
}

GameObject::GameObject(std::string _name, bool _active)
{
	CreateComponent(ComponentType::TRANSFORM);
	name = _name;
	active = _active;
	components = {};
}

GameObject::GameObject(std::string _name, GameObject* _parent, bool _active)
{
	CreateComponent(ComponentType::TRANSFORM);
	name = _name;
	parent = _parent;
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
	for(int i = 0; i<components.size(); i++)
	{
		if (components.at(i)->active)
		{
			components.at(i)->Update();
		}
	}
}

Component* GameObject::CreateComponent(ComponentType type)
{
	bool nonTransform = true; //Only one transform can exist at a time
	Component* ret = nullptr;
	switch (type)
	{
	case TRANSFORM:

		nonTransform = !HasComponent(ComponentType::TRANSFORM);

		if (nonTransform && objTransform == nullptr)
		{
			LOG("Create Component Transform")				
			ret = new ComponentTransform(this);
			components.push_back(ret);
			objTransform = (ComponentTransform*)ret;
		}

		break;
	case MESH:

		//Create component mesh
		LOG("Create Component Mesh");
		ret = new ComponentMesh(this);
		components.push_back(ret);

		break;
	case MATERIAL:

		//Create component material
		LOG("Create Component Transform")
		ret = new ComponentMaterial(this);
		components.push_back(ret);

		break;
	default:
		break;
	}
	
	return ret;
}

bool GameObject::HasComponent(ComponentType type)
{
	bool ret = false;
	for (int i = 0; i < components.size(); i++)
	{
		if (components.at(i)->type == type) { ret = false; }
	}
	return ret;
}

int GameObject::GetComponentPosition(Component* comp)
{
	int ret = -1; //If not in the list
	for (int i = 0; i < components.size(); i++)
	{
		if (components.at(i) == comp) { ret = i; }
	}
	return ret;
}

Component* GameObject::GetComponentByType(ComponentType _type, int num)
{
	Component* ret = nullptr;
	
	int pos = 0;

	if(num>=0)
	{
		
		for (int i = 0; i<components.size() ; i++)
		{
			if(components.at(i)->type == _type)
			{
				if (pos == num)
				{
					ret = components.at(i);
				}
				pos++;
			}
		}
	}
	else
	{
		for (int i = components.size(); i > 0; i--)
		{
			if (components.at(i)->type == _type)
			{
				pos++;
				if (pos == num)
				{
					ret = components.at(i);
				}
			}
		}
	}
	

	return nullptr;
}


