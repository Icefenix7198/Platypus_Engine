#include "ImporterFBX.h"
#include "Globals.h"

#include "ImporterMesh.h"
#include "ResourceMesh.h"
#include "Application.h"

#include "SDL\include\SDL.h"


#pragma comment (lib, "PhysFS/libx86/physfs.lib")

void ImporterFBX::Import(const char* pathFile)
{
	std::vector<std::string> metas;
	App->fileSystem->GetAllFilesWithExtension(ASSETS_MODELS, "meta", metas);
	std::string pathName = App->fileSystem->GetNameFromPath(pathFile, false);
	bool metaAlreadyExists = false;
	for (size_t i = 0; i < metas.size(); i++)
	{
		std::string metaWithoutMeta = App->fileSystem->GetNameFromPath(metas.at(i).c_str());
		if (strcmp(pathName.c_str(), metaWithoutMeta.c_str()) == 0) { metaAlreadyExists = true; }
	}
	if (metaAlreadyExists)
	{
		//If meta already exist is because was already saved, so we try to load it directly
	}
	else
	{
		if(!App->fileSystem->Exists(pathFile))
		{
			std::string originInspector;
			originInspector.assign(pathFile);
			int pos = originInspector.find("Assets");
			if (pos == -1)
			{
				std::string str;
				App->fileSystem->DuplicateFile(pathFile,ASSETS_MODELS,str);
			}
		
			//App->fileSystem->AddToAssets(pathFile, ASSETS_MODELS); //Doesen't work
		}

		ImporterMesh meshImporter;
		meshImporter.Import(pathFile);
	}
	
}

__int64 ImporterFBX::Save()
{
	return 0;
}

void ImporterFBX::Load(char* buffer)
{
	ResourceMesh* reMesh;
	ImporterMesh impMesh;
	impMesh.Load(reMesh, buffer);
}
