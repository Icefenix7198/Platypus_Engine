#include "GameObject.h"
#include "Component.h"

#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ComponentParticleSystem.h"

GameObject::GameObject()
{
	name = "GameObject";
	active = true;
	components = {};
	CreateComponent(ComponentType::TRANSFORM);
}

GameObject::GameObject(std::string _name, bool _active)
{
	name = _name;
	active = _active;
	components = {};
	CreateComponent(ComponentType::TRANSFORM);
}

GameObject::GameObject(std::string _name, GameObject* _parent, bool _active)
{
	name = _name;
	parent = _parent;
	active = _active;
	UUID = App->resources->GenerateNewUID();
	CreateComponent(ComponentType::TRANSFORM);
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
	//delete this;
}

void GameObject::Update()
{
	for(int i = 0; i<components.size(); i++)
	{
		if (components.at(i)->GetActive())
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
			ret->type = type;
			components.push_back(ret);
			objTransform = (ComponentTransform*)ret;
		}

		break;
	case MESH:

		//Create component mesh
		LOG("Create Component Mesh");
		ret = new ComponentMesh(this);
		ret->type = type;
		components.push_back(ret);

		break;
	case MATERIAL:

		//Create component material
		LOG("Create Component Transform")
		ret = new ComponentMaterial(this);
		ret->type = type;
		components.push_back(ret);

		break;
	case CAMERA:

		//Create component material
		LOG("Create Component Camera")
		ret = new ComponentCamera(this);
		ret->type = type;
		components.push_back(ret);
		break;

	case PARTICLE:

		//Create component material
		LOG("Create Component Particle")
		ret = new ComponentParticleSystem(this);
		ret->active = true;
		ret->type = type;
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
		if (components.at(i)->type == type) 
		{ 
			ret = true;
		}
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
	

	return ret;
}

int GameObject::GetNumComponentOfAKind(ComponentType _type)
{
	int ret = 0;

	for (int i = 0; i < components.size(); i++)
	{
		if (components.at(i)->type == _type)
		{
			ret++;
		}
	}

	return ret;
}


