#ifndef __MESH_H__
#define __MESH_H__

#include "MathGeoLib\MathGeoLib.h"
#include "Globals.h"

struct Mesh
{
	// Vertices
	unsigned int id_vertices = 0; // id in VRAM
	unsigned int num_vertices = 0;
	math::float3 *vertices = nullptr;

	// Normals
	unsigned int id_normals = 0; // id in VRAM
	unsigned int num_normals = 0;
	math::float3 *normals = nullptr;

	//// Colors
	//unsigned int id_colors = 0; // id in VRAM
	//unsigned int num_colors = 0;
	//math::float4 *colors = nullptr;

	// Tex coordinates
	unsigned int id_tex_coord = 0; // id in VRAM
	unsigned int num_tex_coord = 0;
	math::float2 *tex_coord = nullptr;

	// Indices
	unsigned int id_indices = 0; // id in VRAM
	unsigned int num_indices = 0;
	unsigned int* indices = nullptr;

	~Mesh()
	{
		/*RELEASE_ARRAY(vertices);
		RELEASE_ARRAY(normals);
		RELEASE_ARRAY(tex_coord);
		RELEASE_ARRAY(indices);*/

		delete[] vertices;
		delete[] normals;
		delete[] tex_coord;
		delete[] indices;
	}
};

#endif //!__MESH_H__

