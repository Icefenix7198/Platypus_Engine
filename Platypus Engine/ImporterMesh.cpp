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
