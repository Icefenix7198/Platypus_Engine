#include "ImporterMaterial.h"
#include "Globals.h"

#include "ResourceMaterial.h"
#include "Application.h"

#include "SDL\include\SDL.h"
#include "PhysFS\include\physfs.h" //Works better than direct.h
#include "parson-master/parson.h"

#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"



//GameObjects creation
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#pragma comment (lib, "PhysFS/libx86/physfs.lib")
#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")

void ImporterMaterial::Import(const char* file_path)
{
	ResourceMaterial* texture = nullptr;
	ILuint new_image_id = 0;
	ilGenImages(1, &new_image_id);
	ilBindImage(new_image_id);

	//Init DevIL to work with OpenGL
	/*if(!ilutRenderer(ILUT_OPENGL))
	{
		LOG("Error initializing DevIL for OpenGL")
	}*/
	//ilutRenderer(ILUT_OPENGL);

	if (ilLoadImage(file_path))
	{
		//iluFlipImage(); 
		std::string str;
		str.assign(file_path);
		/*ILubyte* textdata = ilGetData();*/
		texture->tex = _Texture(str, ilutGLBindTexImage(), (int)ilGetInteger(IL_IMAGE_WIDTH), (int)ilGetInteger(IL_IMAGE_HEIGHT));
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, texture->tex.textureBuffer);

		//Assign how the texture will be managed / scaled / things
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//Unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);


		if (App->scene->selectedGO != App->scene->root)
		{
			auto go = App->scene->selectedGO;
			if (go->HasComponent(ComponentType::MATERIAL))
			{
				ComponentMaterial* cm = (ComponentMaterial*)go->GetComponentByType(ComponentType::MATERIAL);
				cm->textureBuffer = texture->tex.textureBuffer;
				cm->name = texture->name;
				cm->width = texture->tex.width;
				cm->heigth = texture->tex.heigth;
			}
			else
			{
				ComponentMaterial* cm = (ComponentMaterial*)go->CreateComponent(ComponentType::MATERIAL);
				cm->textureBuffer = texture->tex.textureBuffer;
			}
		}

		LOG("Texture successfully loaded: %s", file_path);
	}
	else {
		LOG("Error while loading image in %s", file_path);
		LOG("Error: %s", ilGetString(ilGetError()));
	}

	//Clean memory
	ilDeleteImages(1, &new_image_id);
}

uint64_t ImporterMaterial::Save(ResourceMaterial* resMaterial, char** buffer)
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


