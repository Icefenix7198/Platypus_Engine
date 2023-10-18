#include "aasimp.h"

//#include <vector>

void aasimp::Load(const char* file_path)
{
	Mesh* ourMesh = new Mesh();
	//Creo que hay que iterar sobre una iaMesh a la cual assignemows algo
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene-> HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++) //For e
		{
			//Copy Vertex
			ourMesh->num_vertex = scene->mMeshes[i]->mNumVertices; //Copy the num of Vertex to our mesh from the imported mesh
			
			ourMesh->vertex = new float[ourMesh->num_vertex * 3]; //Create array of vertex of size equal to the imported mesh
			memcpy(ourMesh->vertex, scene->mMeshes[i]->mVertices, sizeof(float) * ourMesh->num_vertex * 3); //Copy the vertices array into mesh Vertex array 
			
			LOG("New mesh with %d vertices", ourMesh->num_vertex);
		
			//Copy faces/indexes
			if (scene->mMeshes[i]->HasFaces())
			{
				ourMesh->num_index = scene->mMeshes[i]->mNumFaces * 3; //Get the number of indixes (it will be 3 for each triangle/face)

				ourMesh->index = new uint[ourMesh->num_index]; // assume each face is a triangle
				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}	
					else
					{
						memcpy(&ourMesh->index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint)); //Copy in index array 
					}
						
				}
			}



			//Copiar OurMesh a un vector de meshes
			vecMeshes.push_back(ourMesh);
		}
		
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene % s", file_path);
	}
		
}
