#include "ImporterMesh.h"
#include "Globals.h"

#include "ResourceMesh.h"

#include "SDL\include\SDL.h"
#include "PhysFS\include\physfs.h" //Works better than direct.h

#pragma comment (lib, "PhysFS/libx86/physfs.lib")

void ImporterMesh::Import()
{
	
}

uint64_t ImporterMesh::Save(ResourceMesh resMesh)
{
	Mesh mesh = resMesh.rMesh;
	//aType of info will be saved
	uint ranges[4] = { mesh.num_index, mesh.num_vertex,mesh.num_normals,mesh.num_UVs };
	//Size of all the CFF document (header + number of each variable)
	uint size = sizeof(ranges) + sizeof(uint) * mesh.num_index + sizeof(float) * mesh.num_vertex * 3 + sizeof(float) * mesh.num_normals * 3 + sizeof(float) * mesh.num_UVs *2; //
	char* fileBuffer = new char[size]; // Allocate
	char* cursor = fileBuffer;
	uint bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);
	cursor += bytes;
	// Store indices
	bytes = sizeof(uint) * mesh.num_index;
	memcpy(cursor, mesh.index, bytes);
	cursor += bytes;
	//Store vertex
	bytes = sizeof(float) * mesh.num_vertex * 3;
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

void ImporterMesh::Load()
{
}
