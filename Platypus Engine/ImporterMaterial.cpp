#include "ImporterMaterial.h"
#include "Globals.h"

//#include "ResourceMesh.h"
#include "Application.h"

#include "SDL\include\SDL.h"
#include "PhysFS\include\physfs.h" //Works better than direct.h
#include "parson-master/parson.h"

//GameObjects creation
#include "ComponentMesh.h"
#include "ComponentTransform.h"

#pragma comment (lib, "PhysFS/libx86/physfs.lib")

void ImporterMaterial::Import(const char* file_path)
{

}

uint64_t ImporterMaterial::Save(/*ResourceMesh* resMesh,*/ char** buffer)
{
	uint64_t size = 0;
	return size;
}

void ImporterMaterial::Load(/*ResourceMesh* resMesh,*/ char* buffer)
{

}

void ImporterMaterial::CreateMetaTexture(const char* filePath,std::vector<Resource*> meshes)
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


