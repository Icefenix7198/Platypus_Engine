#include "ImporterFBX.h"
#include "Globals.h"

#include "ImporterMesh.h"
#include "ResourceMesh.h"
#include "Application.h"

#include "SDL\include\SDL.h"


#pragma comment (lib, "PhysFS/libx86/physfs.lib")

void ImporterFBX::Import(const char* pathFile)
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
