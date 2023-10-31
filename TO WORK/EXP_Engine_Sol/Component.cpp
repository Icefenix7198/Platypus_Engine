#include "Component.h"


Component::Component()
{
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

void Component::OnEditor()
{
}
