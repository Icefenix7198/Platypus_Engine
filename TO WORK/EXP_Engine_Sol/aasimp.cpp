#include "aasimp.h"
#include "GameObject.h"

#include "Application.h"

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

			//Copy normals
			if (scene->mMeshes[i]->HasNormals())
			{
				ourMesh->num_normals = scene->mMeshes[i]->mNumVertices; //Get the number of normals (it will be 3 for each triangle/face)

				ourMesh->normals = new uint[ourMesh->num_normals]; // assume each face is a triangle (so it will have 3 normals)
				memcpy(ourMesh->normals, scene->mMeshes[i]->mNormals, sizeof(float) * ourMesh->num_normals); //Copy the vertices array into mesh Vertex array 

				LOG("New mesh with %d normals", ourMesh->num_normals);
				
			}
			


			//BUFFERS
			ourMesh->VBO = 0; //Buffer de vertices
			ourMesh->VN = 0;
			ourMesh->EBO = 0;
			
			//ourMesh->VAO = 0;

			//Generate buffers.If after this any of them is 0 there is an error
			glGenBuffers(1, &ourMesh->VBO);
			glGenBuffers(1, &ourMesh->VN);
			glGenBuffers(1, &ourMesh->EBO);

			

			glBindBuffer(GL_ARRAY_BUFFER, ourMesh->VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*ourMesh->num_vertex*3, ourMesh->vertex, GL_STATIC_DRAW); 
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER, ourMesh->VN);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ourMesh->num_normals, ourMesh->normals, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ourMesh->EBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * ourMesh->num_index, ourMesh->index, GL_STATIC_DRAW); 
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	


			//Aqui habria que crear un GameObject y asignarle la mesh
			//scene->mMeshes[i].
			//GameObject* go = new GameObject();
			//scene->mRootNode->mTransformation.Decompose();

			//Copiar OurMesh a un vector de meshes
			vecMeshes.push_back(ourMesh);
		}
		
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", file_path);
	}
		
}

void HierarcyGameObject(aiNode* root,std::string name)
{
	if(root->mNumChildren>1)
	{
		//Call recursivelly the function
	}
	else
	{
		if (root->mNumChildren>0)
		{
			HierarcyGameObject(root->mChildren[0],name);
		}
		else
		{
			GameObject* go = new GameObject();
		}
	}
}
