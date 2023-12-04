#include "Importer.h"
#include <direct.h>
#include "Globals.h"

#include "Application.h"
#include "ModuleFileSystem.h"

//List of importers
#include "ImporterFBX.h"
#include "ImporterMesh.h"

#include "aasimp.h"

#include "SDL\include\SDL.h"
#include "PhysFS\include\physfs.h" //Works better than direct.h

#pragma comment (lib, "PhysFS/libx86/physfs.lib")

void Importer::Import(ImporterType type, const char* pathFile)
{
	LOG("File Dropped with path %s", pathFile);

	// Gives the path with only the document no the origin (aka without the C:User/ etc , only the /document.coso
	/*std::string finalPath; //Esto aqui no, se hace en el save
	App->fileSystem->SplitFilePath(pathFile, nullptr, &finalPath); */
		
	std::string normalizedPath = App->fileSystem->NormalizePath(pathFile);

	ImporterFBX importerFBX;
	switch (type)
	{
	case IMPORTER_FBX:
		
		importerFBX.Import(normalizedPath.c_str());
		break;
	case IMPORTER_MESH:
		break;
	case IMPORTER_MATERIAL:
		//TEMPORAL, HABRA QUE HACERLO CON EL IMPORTER
		aasimp::LoadTexture(normalizedPath.c_str());
		break;
	case NUM_IMPORTERS:
		break;
	default:
		break;
	}
}

__int64 Importer::Save()
{
	return 0;
}

void Importer::Load()
{
}
