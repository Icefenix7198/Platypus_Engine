#include "ModuleResourceManager.h"

#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleGameObjectManager.h"
#include "ModuleInput.h"

//Importers
#include "TextureImporter.h"
#include "MeshImporter.h"
#include "SceneImporter.h"
#include "ShaderImporter.h"

// Resources
#include "ResourceTexture.h"
#include "ResourceMaterial.h"
#include "ResourceMesh.h"
#include "ResourceScene.h"

//Components
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ComponentMesh.h"

//Assimp
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

//#include <fstream>
#include <stack>

#pragma comment (lib, "Source/Assimp/libx86/assimp.lib")

ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "Resource Manager");

	check_interval = 2.0f;
	check_timer.Start();	
}

ModuleResourceManager::~ModuleResourceManager()
{ }

bool ModuleResourceManager::Awake(JSONParser &config)
{
	char *buf;
	if (App->file_system->Load("Library/Resources.dat", &buf) != 0)
	{
		// Creating resources for files already imported on last session
		JSONParser resources_info(buf);
		for (int i = 0; i < resources_info.GetArrayCount("Files"); ++i)
		{
			JSONParser item = resources_info.GetArray("Files", i);
			Resource *res = CreateNewResource((RESOURCE_TYPE)item.GetInt("Type"), item.GetUUID("ID"), item.GetInt("Timestamp"));
			res->file = item.GetString("File");
			res->imported_file = item.GetString("Imported File");

			if (res != nullptr) resources[item.GetUUID("ID")] = res;
		}

		RELEASE_ARRAY(buf);
	}

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleResourceManager::Start()
{
	// Shader testing --->
	ShaderImporter::Load("DefaultVertexShader.glsl");
	// ---> Shader testing

	return true;
}

UPDATE_STATUS ModuleResourceManager::PreUpdate(float dt)
{
	if (check_timer.ReadSec() > check_interval)
	{
		std::vector<std::string> files, dirs;
		App->file_system->ExploreFiles(ASSETS, files, dirs, true);

		bool resources_changed = false;

		// Getting new files or overwritten files
		for (uint i = 0; i < files.size(); ++i)
		{
			ID id = Find(files[i]);
			if (id == 0)
			{
				ImportFile(files[i]);			// Asset new -> import!
				resources_changed = true;
			}				
			else if (IsUpdated(id))
			{
				ID id_removed = DeleteImportedFile(files[i]);  // Asset updated -> reimport!
				ImportFile(files[i], id_removed);
				resources_changed = true;
			}
		}

		if (resources_changed)
			CreateJSONResourceInfo();
	}	
	
	// Loading FBXs
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		//LoadFile("cube_trans.fbx");
		LoadFile("primitives_with_parent.fbx");
		//LoadFile("Models/aabb_test.fbx");
		//LoadFile("Street environment_V01.FBX");
		//LoadFile("Models/QuadTree_test3.fbx");
		//LoadFile("Models/color_cubes.fbx");
		//LoadFile("Models/conflict_octree.fbx");
		//LoadFile("Models/a_lot_of_balls2.fbx");
		App->gameobject_manager->UpdateOcTree();
	}

	return UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleResourceManager::PostUpdate(float dt)
{
	if (check_timer.ReadSec() > check_interval)
	{
		FreeInactiveResources();
		check_timer.Start(); // Resetting timer!
	}

	return UPDATE_CONTINUE;
}

bool ModuleResourceManager::CleanUp()
{
	for (std::map<ID, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
		delete (*it).second; //RELEASE((*it).second);
	
	resources.clear();

	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

Resource *ModuleResourceManager::CreateNewResource(RESOURCE_TYPE type, ID id, int timestamp)
{
	Resource *res = nullptr;

	switch (type)
	{
	case (RESOURCE_TYPE::RES_TEXTURES): res = (Resource*) new ResourceTexture(id, timestamp); break;
	case (RESOURCE_TYPE::RES_MATERIAL): res = (Resource*) new ResourceMaterial(id, timestamp); break;
	case (RESOURCE_TYPE::RES_SCENES): res = (Resource*) new ResourceScene(id, timestamp); break;
	case (RESOURCE_TYPE::RES_MESHES): res = (Resource*) new ResourceMesh(id, timestamp); break;	
	}

	if (res != nullptr)
		resources[id] = res;

	return res;
}

ID ModuleResourceManager::ImportFile(std::string &asset_file, ID force_id)
{
	ID res_id = (force_id != 0 ? force_id : GenerateID());	

	switch (GetTypeOfFile(asset_file))
	{
		case(RESOURCE_TYPE::RES_TEXTURES):
		{		
			TextureImporter::Import(asset_file, res_id);
			break;
		}
		case(RESOURCE_TYPE::RES_SCENES):
		{		
			SceneImporter::Import(asset_file, res_id);
			break;
		}
		case(RESOURCE_TYPE::RES_NONE):
			DEBUG("Unknwon type for file %s", asset_file.c_str());
			break;
	}

	return res_id;
}

ID ModuleResourceManager::DeleteImportedFile(std::string &asset_to_delete)
{
	ID id_removed = Find(asset_to_delete);

	Resource *res = Get(id_removed);
	App->file_system->RemoveFile(res->imported_file.c_str());
	DeleteEntry(id_removed);

	return id_removed;
}

ID ModuleResourceManager::GenerateID()
{
	return id_generator.Int();
}

RESOURCE_TYPE ModuleResourceManager::GetTypeOfFile(const std::string &file) const
{
	RESOURCE_TYPE type = RESOURCE_TYPE::RES_NONE;

	std::string tex_extensions[] = { "png", "tga", "jpg", "PNG", "TGA", "JPG"};
	std::string scene_extensions[] = { "fbx", "FBX" };

	size_t dot_pos = file.find_last_of(".");	

	if (dot_pos != std::string::npos)  // Checking dot extension existence
	{
		std::string ext(&file[dot_pos + 1]);  // Setting extension on ext variable

		// Checking textures extensions...
		for (uint i = 0; i < sizeof(tex_extensions) / sizeof(std::string); ++i)
		{
			if (tex_extensions[i] == ext)
			{
				type = RESOURCE_TYPE::RES_TEXTURES;
				break;
			}
		}

		// Checking scene extensions...
		for (uint i = 0; i < sizeof(scene_extensions) / sizeof(std::string); ++i)
		{
			if (scene_extensions[i] == ext)
			{
				type = RESOURCE_TYPE::RES_SCENES;
				break;
			}
		}
	}
	
	return type;
}

ID ModuleResourceManager::Find(const std::string &asset_to_find) const
{
	for (std::map<ID, Resource*>::const_iterator it = resources.begin(); it != resources.end() ;++it)
	{
		if (!(*it).second->file.compare(asset_to_find))
			return (*it).first;
	}

	return 0;
}

void ModuleResourceManager::DeleteEntry(ID id)
{
	std::map<ID,Resource*>::iterator it = resources.find(id);
	RELEASE(it->second);
	resources.erase(it);
}

bool ModuleResourceManager::IsUpdated(ID id) const
{
	Resource *res = resources.at(id);
	int timestamp = 0;

	switch (res->type)
	{
		case(RESOURCE_TYPE::RES_TEXTURES): timestamp = App->file_system->GetLastTimeMod(res->file.c_str(), "Textures/"); break;
		//case(RESOURCE_TYPE::RES_MESHES): timestamp = App->file_system->GetLastTimeMod(res->file.c_str(), LIBRARY_MESH); break;
		case(RESOURCE_TYPE::RES_SCENES): timestamp = App->file_system->GetLastTimeMod(res->file.c_str(), "Models/"); break;
	}

	if (timestamp > res->timestamp)
		return true;
	return false;
}

Resource *ModuleResourceManager::Get(ID id)
{
	std::map<ID, Resource*>::iterator it = resources.find(id);
	if (it != resources.end())
		return resources.at(id);
	return nullptr;
}

bool ModuleResourceManager::LoadFile(const char *file_to_load)
{
	const char *file = App->file_system->GetFileFromDirPath(file_to_load);
	ID id = Find(file);
	if (id == 0)
	{
		DEBUG("File %s has not been imported! Could not be loaded", file);
	}		
	else
	{
		// Getting Scene Resource in order to load all of its components
		Resource *res = Get(Find(file));
		
		char *buf;
		std::string lib_folder = LIBRARY_SCENES;
		App->file_system->Load((lib_folder + res->imported_file).c_str(), &buf);
		JSONParser json_scene; json_scene.ParseBuffer(buf);

		for (int i = 0; i < json_scene.GetArrayCount(file); ++i)
		{
			JSONParser item = json_scene.GetArray(file, i);
			
			switch ((RESOURCE_TYPE)item.GetInt("Type"))
			{
			case(RESOURCE_TYPE::RES_MESHES):
				{
					ResourceMesh *res_mesh = (ResourceMesh*)resources[item.GetUUID("ID")];
					if (!res_mesh->LoadedInMemory())
						MeshImporter::Load(item.GetString("Imported File"), res_mesh);						
							
					res_mesh->LoadToMemory();
					break;
				}
			case(RESOURCE_TYPE::RES_TEXTURES):
				{
					ResourceTexture *res_mat = (ResourceTexture*)resources[item.GetUUID("ID")];
					if (!res_mat->LoadedInMemory())				
						TextureImporter::Load(item.GetString("Imported File"), res_mat);
					
					res_mat->LoadToMemory();
					break; 					
				}
			}
		}

		// Creating GameObjects and their corresponding compoments
		lib_folder = "Models/";
		const aiScene* scene = aiImportFileEx((lib_folder + res->file).c_str(), aiProcessPreset_TargetRealtime_MaxQuality, App->file_system->GetAssimpIO());
		
		if (scene != nullptr)
		{
			if (scene->HasMeshes())
			{
				std::stack<aiNode*> nodes_stack;
				std::stack<GameObject*> go_stack;

				nodes_stack.push(scene->mRootNode);
				go_stack.push((GameObject*)App->gameobject_manager->GetRoot());

				aiNode *curr_node;
				GameObject *parent;

				while (!nodes_stack.empty())
				{
					curr_node = nodes_stack.top();
					parent = go_stack.top();

					int num_children = curr_node->mNumChildren;

					if (num_children > 0)
					{
						nodes_stack.pop();  // Node checked is eliminated.
						go_stack.pop();

						for (int i = 0; i < num_children; ++i)
						{
							aiNode *node_to_add = curr_node->mChildren[i];

							nodes_stack.push(node_to_add);
							char *go_name = node_to_add->mName.data;

							// I don't like! This part eliminates the strange names on Ricard's FBX,
							// but it is exclusive for his FBX, so a better and generic option has to be implemented.						
							for (uint i = 0; i < node_to_add->mName.length; ++i)
							{
								if (go_name[i] == '$')
								{
									go_name[i - 1] = '\0';
									break;
								}
							}

							GameObject *new_go = App->gameobject_manager->CreateGameObject(go_name, parent);
							go_stack.push(new_go);

							// --- TRANSFORM ---					
							ComponentTransform *comp_trans = (ComponentTransform*)new_go->AddComponent(COMPONENT_TYPE::COMP_TRANSFORM);
							comp_trans->SetComponent(node_to_add);

							// --- MESH ---	
							for (uint j = 0; j < node_to_add->mNumMeshes; ++j)
							{
								aiMesh *ai_mesh = scene->mMeshes[node_to_add->mMeshes[j]];
							
								ComponentMesh *comp_mesh = (ComponentMesh*)new_go->AddComponent(COMPONENT_TYPE::COMP_MESH);
								comp_mesh->AddResource((ResourceMesh*)Get(FindFileIdJSON(json_scene, file, go_name)));	

								// --- MATERIAL ---	
								aiMaterial *ai_material = scene->mMaterials[ai_mesh->mMaterialIndex];
								unsigned int numTextures = ai_material->GetTextureCount(aiTextureType_DIFFUSE);
								if (numTextures > 0)
								{
									aiString path;
									ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
									ComponentTexture *comp_mat = (ComponentTexture*)new_go->AddComponent(COMPONENT_TYPE::COMP_TEXTURE);
									comp_mat->AddResource((ResourceTexture*)Get(FindFileIdJSON(json_scene, file, App->file_system->GetFileFromDirPath(path.C_Str()))));
								}				
							}
						}
					}
					else
					{
						go_stack.pop();
						nodes_stack.pop();
					}
				}
			}

			aiReleaseImport(scene);
		}
		else
		{
			DEBUG("Error loading scene %s: %s", file, aiGetErrorString());
		}		
	}	

	return true;
}

void ModuleResourceManager::FreeInactiveResources()
{
	// Freeing buffers that are not used right now.
	for (std::map<ID, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if ((*it).second->GetNumReferences() == 0 && (*it).second->LoadedInMemory())
			(*it).second->UnloadFromMemory();
	}

	// Deleting resources marked as "to delete"
	for (uint i = 0; i < resources_to_delete.size(); ++i)
		delete resources_to_delete[i]; //RELEASE(resources_to_delete[i]);

	resources_to_delete.clear();
}

ID ModuleResourceManager::FindFileIdJSON(const JSONParser &json, const char *scene_name, const char *file) const
{
	for (int i = 0; i < json.GetArrayCount(scene_name); ++i)
	{
		JSONParser item = json.GetArray(scene_name, i);

		if (strcmp(item.GetString("File"), file) == 0)
			return item.GetUUID("ID");
	}

	return 0;
}

void ModuleResourceManager::CreateJSONResourceInfo()
{
	JSONParser resources_info;
	resources_info.AddArray("Files");

	for (std::map<ID, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		JSONParser item;
		item.AddString("File", (*it).second->file.c_str());
		item.AddString("Imported File", (*it).second->imported_file.c_str());
		item.AddUUID("ID", (*it).second->id);
		item.AddInt("Type", (*it).second->type);	
		item.AddInt("Timestamp", (*it).second->timestamp);
		item.AddInt("Times referenced", (*it).second->GetNumReferences());
		resources_info.AddArray(item);
	}

	char *serialized_string;
	resources_info.Save(&serialized_string);
	App->file_system->Save("Library/Resources.dat", serialized_string, strlen(serialized_string));
	resources_info.FreeBuffer(&serialized_string);
}

bool ModuleResourceManager::Save(JSONParser &module)
{
	module.AddArray("Resources");

	for (std::map<ID, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		JSONParser item;
		item.AddString("File", (*it).second->file.c_str());
		item.AddString("Imported File", (*it).second->imported_file.c_str());
		item.AddUUID("ID", (*it).second->id);
		item.AddInt("Type", (*it).second->type);
		item.AddInt("Timestamp", (*it).second->timestamp);
		item.AddBoolean("Loaded in memory", (*it).second->LoadedInMemory());
		item.AddInt("Times referenced", (*it).second->GetNumReferences());

		module.AddArray(item);
	}

	return true;
}

bool ModuleResourceManager::Load(JSONParser &module)
{
	// Cleaning all current resources...
	for (std::map<ID, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		(*it).second->UnloadFromMemory();
		resources_to_delete.push_back((*it).second);
	}
		
	resources.clear();

	// Loading new resources...
	for (int i = 0; i < module.GetArrayCount("Resources"); ++i)
	{
		JSONParser item = module.GetArray("Resources", i);
			
		Resource *res = CreateNewResource((RESOURCE_TYPE)item.GetInt("Type"), item.GetUUID("ID"), item.GetInt("Timestamp"));			
		res->file = item.GetString("File");
		res->imported_file = item.GetString("Imported File");				

		switch (res->type)
		{
		case(RESOURCE_TYPE::RES_MESHES):
			{
				if (item.GetBoolean("Loaded in memory"))
				{
					MeshImporter::Load((std::string)item.GetString("Imported File"), (ResourceMesh*)res);
					res->LoadToMemory();
				}				
				break;
			}
		case(RESOURCE_TYPE::RES_TEXTURES):
			{
				if (item.GetBoolean("Loaded in memory"))
				{
					TextureImporter::Load((std::string)item.GetString("Imported File"), (ResourceTexture*)res);
					res->LoadToMemory();
				}
				break;
			}
		}		
	}

	CreateJSONResourceInfo();

	return true;
}