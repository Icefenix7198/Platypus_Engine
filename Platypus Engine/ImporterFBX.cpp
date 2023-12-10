#include "ImporterFBX.h"
#include "Globals.h"

#include "ImporterMesh.h"
#include "ResourceMesh.h"
#include "Application.h"
#include "parson-master/parson.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"

#include "SDL\include\SDL.h"

#include <vector>

#pragma comment (lib, "PhysFS/libx86/physfs.lib")

void ImporterFBX::Import(const char* pathFile)
{
	std::vector<std::string> metas;
	App->fileSystem->GetAllFilesWithExtension(ASSETS_MODELS, "meta", metas);
	std::string pathName = App->fileSystem->GetNameFromPath(pathFile, false);
	int metaAlreadyExists = -1;
	for (size_t i = 0; i < metas.size(); i++)
	{
		std::string metaWithoutMeta = App->fileSystem->GetNameFromPath(metas.at(i).c_str());
		if (strcmp(pathName.c_str(), metaWithoutMeta.c_str()) == 0) { metaAlreadyExists = i; }
	}
	if (metaAlreadyExists>=0)
	{
		//We read the meta
		std::string metaPath = ASSETS_MODELS + metas.at(metaAlreadyExists);

		//Extract the info
		std::vector<uint32_t> vecUUIDmeshes;
		std::vector<std::string> vecNameMeshes;
		std::string modelName = GetMetaUUID(metaPath.c_str(), vecUUIDmeshes, vecNameMeshes);

		GameObject* go = App->scene->CreateGameObject(App->scene->selectedGO, modelName);

		//Check if all CFF exist compared to the meta
		std::vector<std::string> perrys;
		App->fileSystem->GetAllFilesWithExtension(MESHES_PATH, "pry", perrys);

		//Check each mesh UUID to see what we do with them
		for (size_t i = 0; i < vecUUIDmeshes.size(); i++)
		{
			//We check if its already loaded in the game (Resources Map)
			Resource* reso = App->resources->RequestResource(vecUUIDmeshes.at(i));

			if (reso != nullptr)
			{
				//We create the game Object assigning it the resource
				GameObject* gm = App->scene->CreateGameObject(go, vecNameMeshes.at(i));
				ComponentMesh* cMesh = (ComponentMesh*)gm->CreateComponent(ComponentType::MESH);
				cMesh->resourceMesh = (ResourceMesh*)reso;
			}
			else
			{
				//If not already loaded in engine, we check if is saved in Library
				bool missingPry = false;
				bool smthEqual = false;
				
				for (size_t j = 0; j < perrys.size(); j++)
				{
					uint32_t UUIDLibrary = std::stod(App->fileSystem->GetNameFromPath(perrys.at(j).c_str()));
					if (UUIDLibrary == vecUUIDmeshes.at(i))
					{
						smthEqual = true;
						break;
					}
				
				}
				//After checking all the Library the .pry doesn't exist
				if (!smthEqual)
				{
					missingPry = true;
				}
				if (missingPry)
				{
					//Mesh not found in Library
				}
				else
				{
					GameObject* gm = App->scene->CreateGameObject(go, vecNameMeshes.at(i));
					ComponentMesh* cMesh = (ComponentMesh*)gm->CreateComponent(ComponentType::MESH);

					Resource* resMesh = new ResourceMesh;
					resMesh->type = ResourceType::MESH;
					resMesh->UUID = vecUUIDmeshes.at(i);
					std::string pathIDLibrary;
					pathIDLibrary = MESHES_PATH;
					pathIDLibrary += std::to_string(vecUUIDmeshes.at(i));
					pathIDLibrary += CFF;
					cMesh->resourceMesh->LoadFromLibrary(pathIDLibrary.c_str(), resMesh);
					cMesh->resourceMesh = (ResourceMesh*)resMesh;
				}
			}

			
		}
		
		
	}
	else
	{
		if(!App->fileSystem->Exists(pathFile))
		{
			std::string originInspector;
			originInspector.assign(pathFile);
			int pos = originInspector.find("Assets");
			//We check if the 
			if (pos == -1)
			{
				std::string str;
				App->fileSystem->DuplicateFile(pathFile,ASSETS_MODELS,str);
			}
		
			//App->fileSystem->AddToAssets(pathFile, ASSETS_MODELS); //Doesen't work
		}

		ImporterMesh meshImporter;
		meshImporter.Import(pathFile);
	}
	
}

__int64 ImporterFBX::Save()
{
	return 0;
}

void ImporterFBX::Load(char* buffer)
{
	ResourceMesh* reMesh;
	ImporterMesh impMesh;
	impMesh.Load(reMesh, buffer);
}

const char* ImporterFBX::GetMetaUUID(const char* filePath, std::vector<uint32_t>& vecUUIDmeshes, std::vector<std::string>& vecNameMeshes)
{
	
	JSON_Value* root_value = json_parse_file(filePath);
	JSON_Object* root_object = json_value_get_object(root_value);

	int numMeshes = json_object_get_number(root_object, "NumMeshes");

	JSON_Array* arr = json_object_get_array(root_object, "Meshes");
	for (int i = 0; i < numMeshes; i++)
	{
		vecNameMeshes.push_back(json_array_get_string(arr, i*2));
		vecUUIDmeshes.push_back(json_array_get_number(arr, i * 2+1));
	}
	return json_object_get_string(root_object, "Name");
}
