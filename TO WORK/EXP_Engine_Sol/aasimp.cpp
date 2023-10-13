#include "aasimp.h"

void aasimp::Load(const char* file_path)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && aiScene > HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		aiReleaseImport(scene);
	}
	else
		LOG(“Error loading scene % s”, file_path);
}
