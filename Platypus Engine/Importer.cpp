#include "Importer.h"
#include <direct.h>
#include "Globals.h"

#include "SDL\include\SDL.h"
#include "PhysFS\include\physfs.h" //Works better than direct.h

#pragma comment (lib, "PhysFS/libx86/physfs.lib")

//All other importers


bool Importer::CreateLibrary()
{
	bool ret = true;

	//Initializate physFS
	if (PHYSFS_isInit() == 0)
	{
		char* path = SDL_GetBasePath(); //Get the path of the directory
		if(0==PHYSFS_init(path))
		{
			ret = false;
			LOG("PHYSFS could not initialize! PHYSFS_Error: %s\n", PHYSFS_getLastError());
		}
		SDL_free(path);
	}
	
	//Set writing directory

	//ERIC: Menciona algo de app y org que no entiendo
	char* path2 = SDL_GetBasePath(); //Get the path of the directory
	if (PHYSFS_setWriteDir(path2) == 0)
	{
		LOG("Directory could not be created! PHYSFS_Error: %s\n", PHYSFS_getLastError());
		ret = false;
	}
	else
	{
		LOG("Writing directory is : % s\n" , path2);
	}
	SDL_free(path2);
	if(PHYSFS_mkdir("/Library")==0)
	{
		LOG("Library could not be created! PHYSFS_Error: %s\n", PHYSFS_getLastError());
	}

	

	for (int i = 0; i < ImporterType::NUM_IMPORTERS; i++) //We call the overload of each method that creates its own library
	{
		switch((ImporterType)i)
		{
			case ImporterType::IMPORTER_FBX:
			{

			}
			case ImporterType::IMPORTER_MESH:
			{

			}
			case ImporterType::IMPORTER_MATERIAL:
			{

			}
			default:
			{

			}
		}
	}


	return ret;
}

bool Importer::DestroyLibrary()
{
	bool ret = true;

	

	for (int i = 0; i < ImporterType::NUM_IMPORTERS; i++) //We call the overload of each importer library to destroy its specific library
	{
		switch ((ImporterType)i)
		{
			case ImporterType::IMPORTER_FBX:
			{

			}
			case ImporterType::IMPORTER_MESH:
			{

			}
			case ImporterType::IMPORTER_MATERIAL:
			{

			}
			default:
			{

			}
		}
	}
	
	//Destroy Library

	return ret;
}

void Importer::Import()
{
}

__int64 Importer::Save()
{
	return 0;
}

void Importer::Load()
{
}
