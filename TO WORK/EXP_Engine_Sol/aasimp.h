#pragma once
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"
#include "Glew/include/glew.h"

#include "MathGeoLib/include/MathGeoLib.h"

#include "Globals.h"
//#include "GameObject.h"

#include <vector>

#pragma comment (lib, "Assimp/libx86/assimp.lib")

class GameObject;

struct Mesh 
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	GLuint* index = nullptr;

	uint id_normals = 0; //normal in VRAM
	uint num_normals = 0;
	float* normals = nullptr;
	uint numFaces = 0;
	float* faces = nullptr; //This is necesary to draw faces normals

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
	GLuint VUV;
};



namespace aasimp
{
	void Load(const char* file_path);
	Mesh* ExtractMeshInfo(aiScene* scene);
	
	std::vector<Mesh*> vecMeshes; 
}

void HierarcyGameObject(const aiScene* scene,aiNode* root, const char* name, GameObject* parent);