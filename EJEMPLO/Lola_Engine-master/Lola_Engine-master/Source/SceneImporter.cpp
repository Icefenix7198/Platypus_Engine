// Importers
#include "SceneImporter.h"
#include "TextureImporter.h"
#include "MeshImporter.h"
#include "MaterialImporter.h"

#include "Application.h"
#include "ModuleResourceManager.h"
#include "ModuleFileSystem.h"

// Resources
#include "Resource.h"

// Assimp
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "JSONParser.h"

#include "Globals.h"

#include <stack>

Resource *SceneImporter::Import(const std::string &asset_to_import, const long unsigned int &res_id)
{
	// Imported file
	char imported_filename[SHORT_STRING];
	sprintf_s(imported_filename, SHORT_STRING, "%lu%s", res_id, ".scene");

	// Scene resource
	long unsigned int next_id = res_id;
	int timestamp = App->file_system->GetLastTimeMod(asset_to_import.c_str(), "Models/");
	Resource *scene_res = App->resource_manager->CreateNewResource(RESOURCE_TYPE::RES_SCENES, next_id, timestamp);
	scene_res->file = asset_to_import;
	scene_res->imported_file = imported_filename;

	// Scene entry for JSON file
	JSONParser json_scene;
	json_scene.CreateResourceEntry(scene_res->file.c_str(), scene_res->imported_file.c_str(), scene_res->id, scene_res->type);

	std::string models = "Models/";
	std::vector<std::string> unique_textures;

	const aiScene* scene = aiImportFileEx((models + asset_to_import).c_str(), aiProcessPreset_TargetRealtime_MaxQuality, App->file_system->GetAssimpIO());

	if (scene != nullptr)
	{
		if (scene->HasMeshes())
		{
			json_scene.AddArray(asset_to_import.c_str());

			std::stack<aiNode*> nodes_stack;
			nodes_stack.push(scene->mRootNode);

			aiNode *curr_node;

			while (!nodes_stack.empty())
			{
				curr_node = nodes_stack.top();
				int num_children = curr_node->mNumChildren;

				if (num_children > 0)
				{
					nodes_stack.pop();  // Node checked is eliminated.

					for (int i = 0; i < num_children; ++i)
					{
						aiNode *node_to_add = curr_node->mChildren[i];

						nodes_stack.push(node_to_add);
						char *name = node_to_add->mName.data;

						// I don't like! This part eliminates the strange names on Ricard's FBX,
						// but it is exclusive for his FBX, so a better and generic option has to be implemented.						
						/*for (uint i = 0; i < node_to_add->mName.length; ++i)
						{
							if (name[i] == '$')
							{
								name[i - 1] = '\0';
								break;
							}
						}*/

						// MESH ----->
						for (uint j = 0; j < node_to_add->mNumMeshes; ++j)
						{
							aiMesh *ai_mesh = scene->mMeshes[node_to_add->mMeshes[j]];

							bool accepted = true;
							// Checking for correct number of indices per face. If not, discarded.
							for (uint k = 0; k < ai_mesh->mNumFaces; ++k)
							{
								if (ai_mesh->mFaces[k].mNumIndices != 3)
								{
									DEBUG("WARNING, geometry face with != 3 indices!");
									DEBUG("%s %d %s %s", "Mesh", j, "on", node_to_add->mName.C_Str(), "will not be loaded");
									accepted = false;
								}
							}

							if (accepted)
							{
								Resource *mesh_res = MeshImporter::Import(ai_mesh, ++next_id);
								mesh_res->file = name;
								mesh_res->timestamp = timestamp;								

								// JSON entry for this mesh
								JSONParser mesh;
								mesh.CreateResourceEntry(mesh_res->file.c_str(), mesh_res->imported_file.c_str(), mesh_res->id, mesh_res->type);
								json_scene.AddArray(mesh);
								// -----> MESH 

								// MATERIAL ----->
								aiMaterial *ai_material = scene->mMaterials[ai_mesh->mMaterialIndex];
								Resource *mat_res = MaterialImporter::Import(ai_material, ++next_id);
								mat_res->file = name;
								mat_res->timestamp = timestamp;

								// JSON entry for this material
								JSONParser material;
								material.CreateResourceEntry(mat_res->file.c_str(), mat_res->imported_file.c_str(), mat_res->id, mat_res->type);
								json_scene.AddArray(material);
								// -----> MATERIAL

								// TEXTURES ----->
								if (ai_material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
								{
									// Where is this texture?
									aiString path; ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
									std::string new_texture = App->file_system->GetFileFromDirPath(path.C_Str());

									//Adding new texture if there isn't something similar...
									if (std::find(unique_textures.begin(), unique_textures.end(), new_texture) == unique_textures.end())
									{
										unique_textures.push_back(new_texture);
										Resource *tex_res = TextureImporter::Import(new_texture, ++next_id);

										// JSON entry for this texture
										JSONParser texture;
										texture.CreateResourceEntry(tex_res->file.c_str(), tex_res->imported_file.c_str(), tex_res->id, tex_res->type);
										json_scene.AddArray(texture);
									}
								}
								// -----> TEXTURES
							}
						}
					}
				}
				else
					nodes_stack.pop();
			}

			char *serialized_string;
			json_scene.Save(&serialized_string);
			std::string lib_folder = LIBRARY_SCENES;
			App->file_system->Save((lib_folder + scene_res->imported_file).c_str(), serialized_string, strlen(serialized_string));
			json_scene.FreeBuffer(&serialized_string);
		}

		aiReleaseImport(scene);
	}

	return scene_res;
}
