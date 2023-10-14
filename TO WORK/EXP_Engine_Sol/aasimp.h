#pragma once
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"

#include "Globals.h"

#include <vector>

#pragma comment (lib, "Assimp/libx86/assimp.lib")

struct Mesh 
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
};



namespace aasimp
{
	void Load(const char* file_path);
	static std::vector<Mesh*> vecMeshes; //Quiza hay que hacerlo un objeto static. //Maybe tiene que ser vector<Mesh>* en vez, que sea el vector la ref
}