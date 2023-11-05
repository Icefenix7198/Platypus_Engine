#include "Importer.h"
#include <direct.h>

//All other importers


bool Importer::CreateLibrary()
{
	bool ret = true;

	//Create Library
	if (-1 == mkdir("/Library"))
	{
		ret = false;
		return ret;
	};

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
