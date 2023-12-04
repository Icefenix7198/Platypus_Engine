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

void Resource::SaveToLibrary(Resource* recurso, const char* fileName)
{
	int size;
	char* buffer = nullptr;
	ImporterMesh impMesh;
	std::string libraryPath;
	std::string assetsPath;
	uint32_t ID = App->resources->GenerateNewUID();
	
	//// Gives the path with only the document no the origin (aka without the C:User/ etc , only the /document.coso
	//App->fileSystem->SplitFilePath(filePath, nullptr, &assetsPath);

	switch (recurso->type)
	{
	case ResourceType::TEXTURE:
		break;
	case ResourceType::MESH:
		
		libraryPath.assign(MESHES_PATH);
		libraryPath.append(std::to_string(ID));
		libraryPath.append(".CFF");
		size = impMesh.Save((ResourceMesh*)recurso, &buffer);
		App->fileSystem->Save(libraryPath.c_str(),buffer,size);
		break;
	case ResourceType::NUM_TYPES:
		break;
	default:
		break;
	}

	//After saving the resource in library we load it
}
