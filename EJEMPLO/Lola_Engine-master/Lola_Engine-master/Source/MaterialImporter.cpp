#include "MaterialImporter.h"

#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleResourceManager.h"

#include "ResourceMaterial.h"

#include "Assimp\include\material.h"

Resource *MaterialImporter::Import(const aiMaterial *ai_material, const long unsigned int &res_id)
{
	std::string lib_folder = LIBRARY_MATERIAL;
	std::string imported_file;

	Resource *res = nullptr;

	// Allocate data buffer for color image information and opacity
	uint color_size = sizeof(math::float3) * 5 + sizeof(float);  // 5 colors and opacity
	char *color_data = new char[color_size];					 // allocate data buffer
	char *cursor = color_data;

	aiColor3D ai_color;
	math::float3 color;
	float opacity;

	// We append color and opacity information				
	ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, ai_color);
	color.Set(ai_color.r, ai_color.g, ai_color.b);
	memcpy(cursor, &color, sizeof(math::float3));
	cursor += sizeof(math::float3);

	ai_material->Get(AI_MATKEY_COLOR_SPECULAR, ai_color);
	color.Set(ai_color.r, ai_color.g, ai_color.b);
	memcpy(cursor, &color, sizeof(math::float3));
	cursor += sizeof(math::float3);

	ai_material->Get(AI_MATKEY_COLOR_AMBIENT, ai_color);
	color.Set(ai_color.r, ai_color.g, ai_color.b);
	memcpy(cursor, &color, sizeof(math::float3));
	cursor += sizeof(math::float3);

	ai_material->Get(AI_MATKEY_COLOR_TRANSPARENT, ai_color);
	color.Set(ai_color.r, ai_color.g, ai_color.b);
	memcpy(cursor, &color, sizeof(math::float3));
	cursor += sizeof(math::float3);

	ai_material->Get(AI_MATKEY_COLOR_EMISSIVE, ai_color);
	color.Set(ai_color.r, ai_color.g, ai_color.b);
	memcpy(cursor, &color, sizeof(math::float3));
	cursor += sizeof(math::float3);

	ai_material->Get(AI_MATKEY_OPACITY, opacity);
	memcpy(cursor, &opacity, sizeof(float));

	char imported_filename[100];
	sprintf_s(imported_filename, sizeof(imported_filename), "%lu%s", res_id, ".mtl");
	imported_file = imported_filename;
	if (App->file_system->Save((lib_folder + imported_file).c_str(), color_data, color_size) != 0)
	{
		res = App->resource_manager->CreateNewResource(RESOURCE_TYPE::RES_MATERIAL, res_id, -1);
		res->file = "Material";
		res->imported_file = imported_file;
	}

	RELEASE_ARRAY(color_data);

	return res;
}

unsigned int MaterialImporter::Load(const std::string &imported_file, ResourceMaterial *res_mat)
{
	char *data;
	std::string lib_folder = LIBRARY_MATERIAL;
	uint data_size = App->file_system->Load((lib_folder + imported_file).c_str(), &data);

	if (data_size > 0)
	{
		uint color_size = sizeof(math::float3) * 5 + sizeof(float);  // Size for colors and opacity
		char *cursor = data;

		//res_mat->texture_size = data_size; // -color_size;
		//res_mat->texture_data = new char[res_mat->texture_size];
		//memcpy(res_mat->texture_data, cursor, res_mat->texture_size);
		//cursor += res_mat->texture_size;

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

		memcpy(&res_mat->opacity, cursor, sizeof(float));

		RELEASE_ARRAY(data);
	}

	return data_size;
}