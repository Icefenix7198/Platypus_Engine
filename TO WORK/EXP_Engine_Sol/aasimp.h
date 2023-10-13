#ifndef __Aasimp_H__
#define __Aasimp_H__


#include "aasimp/include/assimp/cimport.h"
#include "aasimp/include/assimp/scene.h"
#include "aasimp/include/assimp/postprocess.h"
#include "aasimp/include/assimp/Importer.hpp"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "Globals.h"

struct vertexData
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
	void Load();
	Has

}

#endif // __Aasimp_H__