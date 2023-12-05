#include "ImporterFBX.h"
#include "Globals.h"

#include "ImporterMesh.h"
#include "ResourceMesh.h"

#include "SDL\include\SDL.h"
#include "PhysFS\include\physfs.h" //Works better than direct.h

#pragma comment (lib, "PhysFS/libx86/physfs.lib")

void ImporterFBX::Import(const char* pathFile)
{
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
