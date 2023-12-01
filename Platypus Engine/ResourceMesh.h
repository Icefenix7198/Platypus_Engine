#pragma once
#include "Resource.h"
#include<string>

class ResourceMesh : public Resource
{
	public:
		ResourceType type;
		uint32_t UUID;

	public:
		ResourceMesh(uint32_t uid, ResourceType type);
		virtual ~ResourceMesh();
};
