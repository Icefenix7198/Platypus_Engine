#include "TextureImporter.h"

#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleResourceManager.h"

#include "ResourceTexture.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment (lib, "Source/Devil/libx86/DevIL.lib")
#pragma comment (lib, "Source/Devil/libx86/ILU.lib")
#pragma comment (lib, "Source/Devil/libx86/ILUT.lib")

Resource *TextureImporter::Import(const std::string &asset_to_import, const long unsigned int &res_id)
{
	ilInit();

	bool success = false;
	Resource *res = nullptr;

	std::string asset_folder = "Textures/";
	std::string lib_folder = LIBRARY_TEXTURE;

	char *data;
	uint size = App->file_system->Load((asset_folder + asset_to_import).c_str(), &data);

	if (size > 0)
	{
		ilLoadL(IL_TYPE_UNKNOWN, data, size);
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // To pick a specific DXT compression use
		
		uint dds_size = ilSaveL(IL_DDS, NULL, 0);
		char *dds_data = new char[dds_size]; // allocate data buffer for DDS file

	    // Save to buffer with the ilSaveIL function
		if (ilSaveL(IL_DDS, dds_data, dds_size) > 0)
		{
			char imported_filename[SHORT_STRING];
			sprintf_s(imported_filename, SHORT_STRING, "%lu%s", res_id, ".dds");			
			std::string imported_file = imported_filename;

			if (App->file_system->Save((lib_folder + imported_file).c_str(), dds_data, dds_size) != 0)
			{
				res = App->resource_manager->CreateNewResource(RESOURCE_TYPE::RES_TEXTURES, res_id, App->file_system->GetLastTimeMod(asset_to_import.c_str(), "Textures/"));
				
				res->file = asset_to_import;
				res->imported_file = imported_file;			
			}
		}

		RELEASE_ARRAY(dds_data);
		RELEASE(data);
	}

	ilShutDown();
	return res;
}

uint TextureImporter::Load(const std::string &imported_file, ResourceTexture *res_mat)
{
	char *data;
	std::string lib_folder = LIBRARY_TEXTURE;
	uint data_size = App->file_system->Load((lib_folder + imported_file).c_str(), &data);

	if (data_size > 0)
	{
		sprintf_s(res_mat->tex_path, SHORT_STRING, "%s%s", "Textures/", res_mat->file.c_str());

		//uint color_size = sizeof(math::float3) * 5 + sizeof(float); // Size for colors and opacity
		char *cursor = data;

		res_mat->texture_size = data_size; // -color_size;
		res_mat->texture_data = new char[res_mat->texture_size];
		memcpy(res_mat->texture_data, cursor, res_mat->texture_size);
		/*cursor += res_mat->texture_size;

		memcpy(&res_mat->color_diffuse, cursor, sizeof(math::float3));
		cursor += sizeof(math::float3);

		memcpy(&res_mat->color_specular, cursor, sizeof(math::float3));
		cursor += sizeof(math::float3);

		memcpy(&res_mat->color_ambient, cursor, sizeof(math::float3));
		cursor += sizeof(math::float3);

		memcpy(&res_mat->color_transparent, cursor, sizeof(math::float3));
		cursor += sizeof(math::float3);

		memcpy(&res_mat->color_emissive, cursor, sizeof(math::float3));
		cursor += sizeof(math::float3);

		memcpy(&res_mat->opacity, cursor, sizeof(float));*/

		RELEASE_ARRAY(data);
	}

	return data_size;
}

uint TextureImporter::Save(unsigned char **data, unsigned int size)
{
	ilInit();

	uint dds_size = 0;

	if (ilLoadL(IL_TYPE_UNKNOWN, *data, size))
	{
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // To pick a specific DXT compression use
		dds_size = ilSaveL(IL_DDS, NULL, 0);

		char *dds_data = new char[dds_size]; // allocate data buffer for DDS and colors

		// Save to buffer with the ilSaveIL function												  
		if (ilSaveL(IL_DDS, dds_data, dds_size) > 0)
			App->file_system->Save("font.dds", dds_data, dds_size);

		RELEASE_ARRAY(dds_data);
	}

	ilShutDown();
	return dds_size;
}

