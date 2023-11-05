#include "Importer.h"

//All other importers


bool Importer::CreateLibrary()
{
	bool ret = true;

	//Create Library

	for (int i = 0; i < ImporterType::NUM_IMPORTERS; i++) //We call the overload of each method that creates its own library
	{
		switch((ImporterType)i)
		{
			case ImporterType::FBX:
			{

			}
			case ImporterType::MESH:
			{

			}
			case ImporterType::MATERIAL:
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
			case ImporterType::FBX:
			{

			}
			case ImporterType::MESH:
			{

			}
			case ImporterType::MATERIAL:
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