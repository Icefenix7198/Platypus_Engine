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
	ResourceType tipo = recurso->type;
	switch (recurso->type)
	{
	case ResourceType::TEXTURE:
		break;
	case ResourceType::MESH:
		size = impMesh.Save((ResourceMesh*)recurso, &buffer);
		App->fileSystem->Save(filePath,buffer,size);
		break;
	case ResourceType::NUM_TYPES:
		break;
	default:
		break;
	}
}
