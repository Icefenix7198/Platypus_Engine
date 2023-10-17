#include "Resource.h"

Resource::Resource()
{
	loaded_in_memory = false;
	times_referenced = 0;
	timestamp = 0;
	id = 0;
	type = RESOURCE_TYPE::RES_NONE;
}

Resource::~Resource()
{ }

bool Resource::LoadToMemory()
{
	return true;
}

bool Resource::UnloadFromMemory()
{
	return true;
}

bool Resource::LoadedInMemory() const
{
	return loaded_in_memory;
}

int Resource::GetNumReferences() const
{
	return times_referenced;
}

void Resource::SetNumReferences(int times)
{
	times_referenced = times;
}

void Resource::IncrReferences()
{
	++times_referenced;
}

void Resource::DecrReferences()
{
	if (times_referenced > 0) --times_referenced;
}
