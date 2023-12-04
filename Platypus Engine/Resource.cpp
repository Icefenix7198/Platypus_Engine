#include "Resource.h"
#include "Application.h"

#include "ImporterMesh.h"

Resource::Resource()
{
}

Resource::Resource(uint32_t uid, ResourceType type)
{

}

Resource::~Resource()
{

}

void Resource::LoadToLibrary(Resource* recurso, const char* filePath)
{
	int size;
	char* buffer = nullptr;
	ImporterMesh impMesh;
	std::string assetsPath;
	switch (recurso->type)
	{
	case ResourceType::TEXTURE:
		break;
	case ResourceType::MESH:
		
		assetsPath.assign(ASSETS_MODELS);
		assetsPath.append(filePath);
		size = impMesh.Save((ResourceMesh*)recurso, &buffer);
		App->fileSystem->Save(assetsPath.c_str(),buffer,size);
		break;
	case ResourceType::NUM_TYPES:
		break;
	default:
		break;
	}
}
