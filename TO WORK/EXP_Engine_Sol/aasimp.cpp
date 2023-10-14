#include "aasimp.h"

void aasimp::Load(const char* file_path)
{
	Mesh ourMesh;
	//Creo que hay que iterar sobre una iaMesh a la cual assignemows algo
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene-> HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++) //For e
		{
			//scene->mMeshes[i]->mVertices
			ourMesh.num_vertex = scene->mMeshes[i]->mNumVertices;
			ourMesh.vertex = new float[ourMesh.num_vertex * 3];
			memcpy(ourMesh.vertex, scene->mMeshes[i]->mVertices, sizeof(float) * ourMesh.num_vertex * 3);
			LOG("New mesh with %d vertices", ourMesh.num_vertex);
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene % s", file_path);
	}
		
}
