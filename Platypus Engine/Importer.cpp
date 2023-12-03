#include "Importer.h"
#include <direct.h>
#include "Globals.h"

//List of importers
#include "ImporterFBX.h"
#include "ImporterMesh.h"

#include "SDL\include\SDL.h"
#include "PhysFS\include\physfs.h" //Works better than direct.h

#pragma comment (lib, "PhysFS/libx86/physfs.lib")

void Importer::Import(ImporterType type, const char* pathFile)
{
	ImporterFBX importerFBX;
	switch (type)
	{
	case IMPORTER_FBX:
		
		importerFBX.Import(pathFile);
		break;
	case IMPORTER_MESH:
		break;
	case IMPORTER_MATERIAL:
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
