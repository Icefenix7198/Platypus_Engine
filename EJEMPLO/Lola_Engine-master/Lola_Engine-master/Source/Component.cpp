#include "Component.h"

#include "Application.h"
#include "ModuleGameObjectManager.h"

#include "JSONParser.h"

Component::Component()
{
	type = COMPONENT_TYPE::COMP_UNKNOWN;
	active = true;

	App->gameobject_manager->GenerateUUID(this);
}

Component::~Component()
{ }

bool Component::Update()
{ 
	return true;
}

void Component::ShowEditorInfo()
{ }

COMPONENT_TYPE &Component::GetType()
{
	return type;
}

const char *Component::GetName()
{
	return name;
}

bool Component::IsActive() const
{
	return active;
}

bool Component::Save(JSONParser &go)
{
	return true;
}

bool Component::Load(JSONParser &component)
{
	return true;
}