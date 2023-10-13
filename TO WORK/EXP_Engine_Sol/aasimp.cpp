#include "aasimp.h"

//#include "aasimp/include/assimp/cimport.h"
//#include "aasimp/include/assimp/scene.h"
//#include "aasimp/include/assimp/postprocess.h"
//#include "aasimp/include/assimp/Importer.hpp"

//// Stream log messages to Debug window
//struct aiLogStream stream;
//stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
//aiAttachLogStream(&stream);

//// detach log stream
//aiDetachAllLogStreams(); //Hacer en el cleanUp

void aasimp::Load()
{
	const aiScene* scene /*= aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality)*/;
	if (scene != nullptr && aiScene > HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		aiReleaseImport(scene);
	}
	else
		LOG(“Error loading scene % s”, file_path);
}
