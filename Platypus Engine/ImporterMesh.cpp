#include "ImporterMesh.h"
#include "Globals.h"

#include "ResourceMesh.h"
#include "Application.h"

#include "SDL\include\SDL.h"
#include "PhysFS\include\physfs.h" //Works better than direct.h
#include "parson-master/parson.h"

//GameObjects creation
#include "ComponentMesh.h"
#include "ComponentTransform.h"

#pragma comment (lib, "PhysFS/libx86/physfs.lib")

void ImporterMesh::Import(const char* file_path)
{

	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		//We save here all the resource meshes we import
		std::vector<Resource*> meshes;
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			ResourceMesh* reMesh = new ResourceMesh;
			reMesh->type = ResourceType::MESH;
			//Copy Vertex
			reMesh->rMesh.num_vertex = scene->mMeshes[i]->mNumVertices; //Copy the num of Vertex to our mesh from the imported mesh

			reMesh->rMesh.vertex = new float[reMesh->rMesh.num_vertex * 3]; //Create array of vertex of size equal to the imported mesh
			memcpy(reMesh->rMesh.vertex, scene->mMeshes[i]->mVertices, sizeof(float) * reMesh->rMesh.num_vertex * 3); //Copy the vertices array into mesh Vertex array 

			LOG("New mesh with %d vertices", reMesh->rMesh.num_vertex);

			//Copy faces/indexes
			if (scene->mMeshes[i]->HasFaces())
			{
				reMesh->rMesh.num_index = scene->mMeshes[i]->mNumFaces * 3; //Get the number of indixes (it will be 3 for each triangle/face)

				reMesh->rMesh.index = new uint[reMesh->rMesh.num_index]; // assume each face is a triangle
				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&reMesh->rMesh.index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint)); //Copy in index array 
					}

				}
			}

			//Copy normals
			if (scene->mMeshes[i]->HasNormals())
			{
				reMesh->rMesh.num_normals = scene->mMeshes[i]->mNumVertices * 3; //Get the number of normals (it will be 3 for each triangle/face)

				reMesh->rMesh.normals = new float[reMesh->rMesh.num_normals]; // assume each face is a triangle (so it will have 3 normals)
				memcpy(reMesh->rMesh.normals, scene->mMeshes[i]->mNormals, sizeof(float) * reMesh->rMesh.num_normals); //Copy the vertices array into mesh Vertex array 

				LOG("New mesh with %d normals", reMesh->rMesh.num_normals);

			}

			// Copy UV
			uint UV_index = 0;
			if (scene->mMeshes[i]->HasTextureCoords(UV_index))
			{
				reMesh->rMesh.num_UVs = reMesh->rMesh.num_vertex;
				reMesh->rMesh.UVs = new float2[reMesh->rMesh.num_UVs];
				for (uint k = 0; k < scene->mMeshes[i]->mNumVertices; k++) //There is one UV per vertex
				{
					reMesh->rMesh.UVs[k].x = scene->mMeshes[i]->mTextureCoords[UV_index][k].x;
					reMesh->rMesh.UVs[k].y = scene->mMeshes[i]->mTextureCoords[UV_index][k].y;
				}
				LOG("New mesh with %d texture coordinates", reMesh->rMesh.num_UVs);
			}

			//BUFFERS
			reMesh->rMesh.VBO = 0; //Buffer de vertices
			reMesh->rMesh.VN = 0;
			reMesh->rMesh.EBO = 0;
			reMesh->rMesh.VUV = 0;

			//reMesh->rMesh.VAO = 0;

			//Generate buffers.If after this any of them is 0 there is an error
			glGenBuffers(1, &reMesh->rMesh.VBO);
			glGenBuffers(1, &reMesh->rMesh.VN);
			glGenBuffers(1, &reMesh->rMesh.EBO);
			glGenBuffers(1, &reMesh->rMesh.VUV);

			glBindBuffer(GL_ARRAY_BUFFER, reMesh->rMesh.VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*reMesh->rMesh.num_vertex*3, reMesh->rMesh.vertex, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER, reMesh->rMesh.VN);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * reMesh->rMesh.num_normals, reMesh->rMesh.normals, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER, reMesh->rMesh.VUV);
			glBufferData(GL_ARRAY_BUFFER, sizeof(math::float2) * reMesh->rMesh.num_UVs, reMesh->rMesh.UVs, GL_STATIC_DRAW);
			App->renderer3D->checkersID = reMesh->rMesh.VUV; //Esto tendra que ir fuera, importamos meshes no texturas.
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, reMesh->rMesh.EBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * reMesh->rMesh.num_index, reMesh->rMesh.index, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			//Once all the info we want is loaded we save the mesh on library
			Resource* recu = (Resource*)reMesh;
			recu->type = ResourceType::MESH;
			recu->name = scene->mRootNode->mChildren[i]->mName.C_Str();
			reMesh->SaveToLibrary(recu, scene->mRootNode->mChildren[i]->mName.C_Str());

			//Add all meshes into an array for the meta
			meshes.push_back(recu);
		}
		//Create Meta (we do it here because there is only one meta for Model no for Mesh, but we need a list of our resources Mesh in the meta)
		CreateMetaModel(file_path,meshes);

		CreateGameObjectHierarchy(scene, scene->mRootNode, App->fileSystem->GetNameFromPath(file_path).c_str(), App->scene->root, meshes);
		aiReleaseImport(scene);
	}
}

uint64_t ImporterMesh::Save(ResourceMesh* resMesh, char** buffer)
{
	_Mesh mesh = resMesh->rMesh;
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
	bytes = sizeof(float) * mesh.num_normals /** 3*/;
	memcpy(cursor, mesh.normals, bytes);
	cursor += bytes;

	//Store UVs
	bytes = sizeof(float) * mesh.num_UVs * 2;
	memcpy(cursor, mesh.UVs, bytes);
	cursor += bytes;

	//We return the size of the buffer for things in the future
	return size;
}

void ImporterMesh::Load(ResourceMesh* resMesh, char* buffer)
{
	char* cursor = buffer;
	// amount of indices / vertices / normals / UVs
	uint ranges[4];

	//Load header
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	resMesh->rMesh.num_index = ranges[0];
	resMesh->rMesh.num_vertex = ranges[1];
	resMesh->rMesh.num_normals = ranges[2];
	resMesh->rMesh.num_UVs = ranges[3];

	// Load index
	bytes = sizeof(uint) * resMesh->rMesh.num_index;
	resMesh->rMesh.index = new uint[resMesh->rMesh.num_index];
	memcpy(resMesh->rMesh.index, cursor, bytes);
	cursor += bytes;

	// Load vertex
	bytes = sizeof(float) * resMesh->rMesh.num_vertex * 3;
	resMesh->rMesh.vertex = new float[resMesh->rMesh.num_vertex*3];
	memcpy(resMesh->rMesh.vertex, cursor, bytes);
	cursor += bytes;

	// Load normals
	bytes = sizeof(float) * resMesh->rMesh.num_normals * 3;
	resMesh->rMesh.normals = new float[resMesh->rMesh.num_normals*3];
	memcpy(resMesh->rMesh.normals, cursor, bytes);
	cursor += bytes;

	// Load UVs
	bytes = sizeof(float) * resMesh->rMesh.num_UVs * 2;
	resMesh->rMesh.UVs = new math::float2[resMesh->rMesh.num_UVs];
	memcpy(resMesh->rMesh.UVs, cursor, bytes);
	cursor += bytes;
}

void ImporterMesh::CreateMetaModel(const char* filePath,std::vector<Resource*> meshes)
{
	JSON_Value* root_value = json_value_init_object();
	JSON_Object* root_object = json_value_get_object(root_value);

	//If there is no failure loading
	if (root_value != nullptr && root_object != nullptr)
	{
		char* serialized_string = NULL;
		//Crear path
		json_object_set_string(root_object, "FilePath", filePath);
		json_object_set_string(root_object, "Name", App->fileSystem->GetNameFromPath(filePath).c_str());
		for (size_t i = 0; i < meshes.size(); i++)
		{
			std::string IDname = meshes.at(i)->name;
			IDname.append(".UUID");
			json_object_dotset_number(root_object, IDname.c_str(), meshes.at(i)->UUID);
		}
		//Dot set hace que si lo pones en un punto te lo ponga dentro de un {} del punto antes del 
		//Jason_parse_string lo mete en un array
		//json_object_dotset_value(root_object, "contact.emails", json_parse_string("[\"email@example.com\",\"email2@example.com\"]"));
		serialized_string = json_serialize_to_string_pretty(root_value);
		puts(serialized_string);
		
		//Crear el archivo en assets
		std::string nameMeta;
		nameMeta += ASSETS_MODELS;
		nameMeta += App->fileSystem->GetNameFromPath(filePath, false);
		nameMeta += ".meta";
		json_serialize_to_file(root_value, nameMeta.c_str());
		json_free_serialized_string(serialized_string);
		json_value_free(root_value);
	}
}

void ImporterMesh::CreateGameObjectHierarchy(const aiScene* scene, aiNode* root, const char* name, GameObject* parent, std::vector<Resource*> meshes)
{
	if (root->mNumChildren > 1)
	{
		if (root->mNumMeshes > 0)
		{
			for (int i = 0; i < root->mNumMeshes; i++)
			{
				//Transform 
				aiVector3D translation, scaling;
				aiQuaternion rotation;
				root->mTransformation.Decompose(scaling, rotation, translation);
				GameObject* gm = App->scene->CreateGameObject(parent, name);
				gm->objTransform->SetValues(translation, scaling, rotation);

				//Mesh
				ComponentMesh* cMesh; //Create reference to modify compoent mesh
				cMesh = (ComponentMesh*)gm->CreateComponent(ComponentType::MESH); //Create component mesh
				for (size_t i = 0; i < meshes.size(); i++)
				{
					if(strcmp(meshes.at(i)->name.c_str(),name) == 0)
					{
						cMesh->resource = (ResourceMesh*)meshes.at(i);
					}
				}
				
				//Material
				ComponentMaterial* cMaterial;
				cMaterial = (ComponentMaterial*)gm->CreateComponent(ComponentType::MATERIAL);
			}

		}
		else
		{
			parent = App->scene->CreateGameObject(parent, name);
		}


		for (int i = 0; i < root->mNumChildren; i++)
		{
			CreateGameObjectHierarchy(scene, root->mChildren[i], root->mChildren[i]->mName.C_Str(), parent, meshes);
		}
	}
	else
	{
		if (root->mNumChildren > 0)
		{
			if (root->mNumMeshes > 0)
			{
				for (int i = 0; i < root->mNumMeshes; i++)
				{
					aiVector3D translation, scaling;
					aiQuaternion rotation;
					root->mTransformation.Decompose(scaling, rotation, translation);
					LOG("Create GamoObject for mesh %s", name);
					GameObject* gm = App->scene->CreateGameObject(parent, name);
					gm->objTransform->SetValues(translation, scaling, rotation);
					ComponentMesh* cMesh; //Create reference to modify compoent mesh
					cMesh = (ComponentMesh*)gm->CreateComponent(ComponentType::MESH); //Create component mesh
					for (size_t i = 0; i < meshes.size(); i++)
					{
						if (strcmp(meshes.at(i)->name.c_str(), name) == 0)
						{
							cMesh->resource = (ResourceMesh*)meshes.at(i);
						}
					}
				}

			}


			HierarcyGameObject(scene, root->mChildren[0], root->mChildren[0]->mName.C_Str(), parent);

		}
		else
		{

			if (root->mNumMeshes > 0)
			{
				for (int i = 0; i < root->mNumMeshes; i++)
				{
					aiVector3D translation, scaling;
					aiQuaternion rotation;
					root->mTransformation.Decompose(scaling, rotation, translation);
					LOG("Create GamoObject for mesh %s", name)
						GameObject* gm = App->scene->CreateGameObject(parent, name);
					gm->objTransform->SetValues(translation, scaling, rotation);
					ComponentMesh* cMesh; //Create reference to modify compoent mesh
					cMesh = (ComponentMesh*)gm->CreateComponent(ComponentType::MESH); //Create component mesh
					for (size_t i = 0; i < meshes.size(); i++)
					{
						if (strcmp(meshes.at(i)->name.c_str(), name) == 0)
						{
							cMesh->resource = (ResourceMesh*)meshes.at(i);
						}
					}
				}

			}

		}
	}
}


void HierarcyGameObject(const aiScene* scene, aiNode* root, const char* name, GameObject* parent)
{
	if (root->mNumChildren > 1)
	{
		if (root->mNumMeshes > 0)
		{
			for (int i = 0; i < root->mNumMeshes; i++)
			{
				//Transform 
				aiVector3D translation, scaling;
				aiQuaternion rotation;
				root->mTransformation.Decompose(scaling, rotation, translation);
				GameObject* gm = App->scene->CreateGameObject(parent, name);
				gm->objTransform->SetValues(translation, scaling, rotation);

				//Mesh
				ComponentMesh* cMesh; //Create reference to modify compoent mesh
				cMesh = (ComponentMesh*)gm->CreateComponent(ComponentType::MESH); //Create component mesh
				cMesh->IDResourceMesh;

				//Material
				ComponentMaterial* cMaterial;
				cMaterial = (ComponentMaterial*)gm->CreateComponent(ComponentType::MATERIAL);
			}

		}
		else
		{
			parent = App->scene->CreateGameObject(parent, name);
		}


		for (int i = 0; i < root->mNumChildren; i++)
		{
			HierarcyGameObject(scene, root->mChildren[i], root->mChildren[i]->mName.C_Str(), parent);
		}
	}
	else
	{
		if (root->mNumChildren > 0)
		{
			if (root->mNumMeshes > 0)
			{
				for (int i = 0; i < root->mNumMeshes; i++)
				{
					aiVector3D translation, scaling;
					aiQuaternion rotation;
					root->mTransformation.Decompose(scaling, rotation, translation);
					LOG("Create GamoObject for mesh %s", name);
					GameObject* gm = App->scene->CreateGameObject(parent, name);
					gm->objTransform->SetValues(translation, scaling, rotation);
					ComponentMesh* cMesh; //Create reference to modify compoent mesh
					cMesh = (ComponentMesh*)gm->CreateComponent(ComponentType::MESH); //Create component mesh
					//uint* numMesh = root->mMeshes; //mMeshes is a number to to the scene mMeshes array
					//cMesh->mesh = AiMeshtoMesh(scene->mMeshes[numMesh[i]]); //TODO ERIC: Es un aiMesh, hay que convertirlo a Mesh normal
				}

			}


			HierarcyGameObject(scene, root->mChildren[0], root->mChildren[0]->mName.C_Str(), parent);

		}
		else
		{

			if (root->mNumMeshes > 0)
			{
				for (int i = 0; i < root->mNumMeshes; i++)
				{
					aiVector3D translation, scaling;
					aiQuaternion rotation;
					root->mTransformation.Decompose(scaling, rotation, translation);
					LOG("Create GamoObject for mesh %s", name)
						GameObject* gm = App->scene->CreateGameObject(parent, name);
					gm->objTransform->SetValues(translation, scaling, rotation);
					ComponentMesh* cMesh; //Create reference to modify compoent mesh
					cMesh = (ComponentMesh*)gm->CreateComponent(ComponentType::MESH); //Create component mesh
					//uint* numMesh = root->mMeshes; //mMeshes is a number to to the scene mMeshes array
					//cMesh->mesh = AiMeshtoMesh(scene->mMeshes[numMesh[i]]); //TODO ERIC: Es un aiMesh, hay que convertirlo a Mesh normal
				}

			}

		}
	}
}