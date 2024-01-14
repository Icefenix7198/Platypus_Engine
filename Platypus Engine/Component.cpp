#include "Component.h"


Component::Component()
{
	active = true;
}

Component::~Component()
{
}

bool Component::Enable()
{
	active = true;
	return true;
}

bool Component::Update()
{
	active = false;
	return true;
}

bool Component::Disable()
{
	return false;
}

bool Component::GetActive()
{
	return active;
}

void Component::OnEditor()
{
}
