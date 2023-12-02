#include "ImporterMesh.h"
#include "Globals.h"

#include "ResourceMesh.h"

#include "SDL\include\SDL.h"
#include "PhysFS\include\physfs.h" //Works better than direct.h

#pragma comment (lib, "PhysFS/libx86/physfs.lib")

void ImporterMesh::Import(aiScene* scene,ResourceMesh &reMesh)
{
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			//Copy Vertex
			reMesh.rMesh.num_vertex = scene->mMeshes[i]->mNumVertices; //Copy the num of Vertex to our mesh from the imported mesh

			reMesh.rMesh.vertex = new float[reMesh.rMesh.num_vertex * 3]; //Create array of vertex of size equal to the imported mesh
			memcpy(reMesh.rMesh.vertex, scene->mMeshes[i]->mVertices, sizeof(float) * reMesh.rMesh.num_vertex * 3); //Copy the vertices array into mesh Vertex array 

			LOG("New mesh with %d vertices", reMesh.rMesh.num_vertex);

			//Copy faces/indexes
			if (scene->mMeshes[i]->HasFaces())
			{
				reMesh.rMesh.num_index = scene->mMeshes[i]->mNumFaces * 3; //Get the number of indixes (it will be 3 for each triangle/face)

				reMesh.rMesh.index = new uint[reMesh.rMesh.num_index]; // assume each face is a triangle
				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&reMesh.rMesh.index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint)); //Copy in index array 
					}

				}
			}

			//Copy normals
			if (scene->mMeshes[i]->HasNormals())
			{
				reMesh.rMesh.num_normals = scene->mMeshes[i]->mNumVertices * 3; //Get the number of normals (it will be 3 for each triangle/face)

				reMesh.rMesh.normals = new float[reMesh.rMesh.num_normals]; // assume each face is a triangle (so it will have 3 normals)
				memcpy(reMesh.rMesh.normals, scene->mMeshes[i]->mNormals, sizeof(float) * reMesh.rMesh.num_normals); //Copy the vertices array into mesh Vertex array 

				LOG("New mesh with %d normals", reMesh.rMesh.num_normals);

			}

			// Copy UV
			uint UV_index = 0;
			if (scene->mMeshes[i]->HasTextureCoords(UV_index))
			{
				reMesh.rMesh.num_UVs = reMesh.rMesh.num_vertex;
				reMesh.rMesh.UVs = new float2[reMesh.rMesh.num_UVs];
				for (uint k = 0; k < scene->mMeshes[i]->mNumVertices; k++) //There is one UV per vertex
				{
					reMesh.rMesh.UVs[k].x = scene->mMeshes[i]->mTextureCoords[UV_index][k].x;
					reMesh.rMesh.UVs[k].y = scene->mMeshes[i]->mTextureCoords[UV_index][k].y;
				}
				LOG("New mesh with %d texture coordinates", reMesh.rMesh.num_UVs);
			}

			//BUFFERS
			reMesh.rMesh.VBO = 0; //Buffer de vertices
			reMesh.rMesh.VN = 0;
			reMesh.rMesh.EBO = 0;
			reMesh.rMesh.VUV = 0;

			//reMesh.rMesh.VAO = 0;

			//Generate buffers.If after this any of them is 0 there is an error
			/*glGenBuffers(1, &reMesh.rMesh.VBO);
			glGenBuffers(1, &reMesh.rMesh.VN);
			glGenBuffers(1, &reMesh.rMesh.EBO);
			glGenBuffers(1, &reMesh.rMesh.VUV);*/

			//TODO: MIRAR SI ESTO ES NECESARIO
			/*glBindBuffer(GL_ARRAY_BUFFER, reMesh.rMesh.VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*reMesh.rMesh.num_vertex*3, reMesh.rMesh.vertex, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER, reMesh.rMesh.VN);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * reMesh.rMesh.num_normals, reMesh.rMesh.normals, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER, reMesh.rMesh.VUV);
			glBufferData(GL_ARRAY_BUFFER, sizeof(math::float2) * reMesh.rMesh.num_UVs, reMesh.rMesh.UVs, GL_STATIC_DRAW);
			App->renderer3D->textureID = reMesh.rMesh.VUV;
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, reMesh.rMesh.EBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * reMesh.rMesh.num_index, reMesh.rMesh.index, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/

			/*glBindBuffer(GL_ARRAY_BUFFER, reMesh.rMesh.id_UVs);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * reMesh.rMesh.num_UVs * 2, reMesh.rMesh.UVs, GL_STATIC_DRAW);
			App->renderer3D->textureID = reMesh.rMesh.id_UVs;
			glBindBuffer(GL_ARRAY_BUFFER, 0);*/




			//Copy reMesh.rMesh to vector of meshes
			//vecMeshes.push_back(reMesh.rMesh);
		}
	}
}

uint64_t ImporterMesh::Save(ResourceMesh resMesh, char** buffer)
{
	Mesh mesh = resMesh.rMesh;
	//aType of info will be saved
	uint ranges[4] = { mesh.num_index, mesh.num_vertex,mesh.num_normals,mesh.num_UVs };
	//Size of all the CFF document (header + number of each variable)
	uint size = sizeof(ranges) + sizeof(uint) * mesh.num_index + sizeof(float) * mesh.num_vertex * 3 + sizeof(float) * mesh.num_normals * 3 + sizeof(float) * mesh.num_UVs *2; //
	*buffer = new char[size]; // Allocate
	char* cursor = *buffer;
	uint bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);
	cursor += bytes;
	// Store indices
	bytes = sizeof(uint) * mesh.num_index;
	memcpy(cursor, mesh.index, bytes);
	cursor += bytes;

	//Store vertex
	bytes = sizeof(float) * mesh.num_vertex * 3; //SARA CONSEJO: Si peta todo probar con uint no float
	memcpy(cursor, mesh.vertex, bytes);
	cursor += bytes;

	//Store normals
	bytes = sizeof(float) * mesh.num_normals * 3;
	memcpy(cursor, mesh.normals, bytes);
	cursor += bytes;

	//Store UVs
	bytes = sizeof(float) * mesh.num_UVs * 2;
	memcpy(cursor, mesh.UVs, bytes);
	cursor += bytes;

	//We return the size of the buffer for things in the future
	return size;
}

void ImporterMesh::Load(ResourceMesh &resMesh, char* buffer)
{
	char* cursor = buffer;
	// amount of indices / vertices / normals / UVs
	uint ranges[4];

	//Load header
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	resMesh.rMesh.num_index = ranges[0];
	resMesh.rMesh.num_vertex = ranges[1];
	resMesh.rMesh.num_normals = ranges[2];
	resMesh.rMesh.num_UVs = ranges[3];

	// Load index
	bytes = sizeof(uint) * resMesh.rMesh.num_index;
	resMesh.rMesh.index = new uint[resMesh.rMesh.num_index];
	memcpy(resMesh.rMesh.index, cursor, bytes);
	cursor += bytes;

	// Load vertex
	bytes = sizeof(float) * resMesh.rMesh.num_vertex * 3;
	resMesh.rMesh.vertex = new float[resMesh.rMesh.num_vertex];
	memcpy(resMesh.rMesh.vertex, cursor, bytes);
	cursor += bytes;

	// Load normals
	bytes = sizeof(float) * resMesh.rMesh.num_normals * 3;
	resMesh.rMesh.normals = new float[resMesh.rMesh.num_normals];
	memcpy(resMesh.rMesh.normals, cursor, bytes);
	cursor += bytes;

	// Load UVs
	bytes = sizeof(float) * resMesh.rMesh.num_UVs * 2;
	resMesh.rMesh.UVs = new math::float2[resMesh.rMesh.num_UVs];
	memcpy(resMesh.rMesh.UVs, cursor, bytes);
	cursor += bytes;
}
