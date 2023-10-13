#include "aasimp.h"

void aasimp::Load(const char* file_path)
{
	Mesh ourMesh;
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene-> HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array

		
		ourMesh.num_vertex /*= aiMesh->mNumVertices*/;
		ourMesh.vertex = new float[ourMesh.num_vertex * 3];
		memcpy(ourMesh.vertex, aiMesh->mVertices, sizeof(float) * ourMesh.num_vertices * 3);
		LOG("New mesh with %d vertices", m.num_vertices);

		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene % s", file_path);
	}
		
}
