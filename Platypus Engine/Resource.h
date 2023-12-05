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
		std::string name;
		ResourceType type;
		uint32_t UUID;

	public:
		Resource();
		Resource(uint32_t uid, ResourceType type);
		virtual ~Resource();
		void SaveToLibrary(Resource* recurso, const char* fileName);
		void LoadFromLibrary(const char* fileName, Resource* resourceToLoad = nullptr);
};
