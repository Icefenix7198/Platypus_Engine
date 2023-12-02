#pragma once

#include<string>

enum class ResourceType 
{
		TEXTURE,
		MESH,
		NUM_TYPES
};
class Resource
{
	public:
		ResourceType type;
		uint32_t UUID;

	public:
		Resource();
		Resource(uint32_t uid, ResourceType type);
		virtual ~Resource();
};
