#pragma once
#include "Resource.h"
#include "Globals.h"
#include "Glew/include/glew.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include<string>

struct _Mesh
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	GLuint* index = nullptr;

	uint id_normals = 0; //normal in VRAM
	uint num_normals = 0;
	float* normals = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	uint id_UVs = 0; // unique vertex in VRAM
	GLuint num_UVs = 0;
	math::float2* UVs = nullptr;


	GLuint VAO;
	GLuint EBO;
	GLuint VBO;
	GLuint VN; //Normales para las texturas
	GLuint VUV; //Vector de UVs
};

class ResourceMesh : public Resource
{
public:
		ResourceMesh();
		ResourceMesh(uint32_t uid, ResourceType type);
		virtual ~ResourceMesh();
public:
	ResourceType type;
	uint32_t UUID;
	_Mesh rMesh;

	
};
