#pragma once
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"
#include "Glew/include/glew.h"

#include "Globals.h"
//#include "GameObject.h"

#include <vector>

#pragma comment (lib, "Assimp/libx86/assimp.lib")

class GameObject;

struct Mesh 
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_normals = 0; //normal in VRAM
	uint num_normals = 0;
	uint* normals = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	GLuint VAO;
	GLuint EBO;
	GLuint VBO;
	GLuint VN; //Normales para las texturas
};



namespace aasimp
{
	void Load(const char* file_path);
	Mesh* ExtractMeshInfo(aiScene* scene);
	
	std::vector<Mesh*> vecMeshes; 
}

void HierarcyGameObject(const aiScene* scene,aiNode* root, const char* name, GameObject* parent);