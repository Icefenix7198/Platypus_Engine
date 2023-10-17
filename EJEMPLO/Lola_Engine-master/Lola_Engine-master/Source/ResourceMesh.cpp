#include "ResourceMesh.h"

#include "openGL.h"

#include "Mesh.h"

ResourceMesh::ResourceMesh(long unsigned int id, int timestamp) : Resource()
{
	this->id = id;
	this->timestamp = timestamp;
	type = RESOURCE_TYPE::RES_MESHES;

	mesh_data = nullptr;
}

ResourceMesh::~ResourceMesh()
{
	if (mesh_data != nullptr)
		RELEASE(mesh_data);
}

bool ResourceMesh::LoadToMemory()
{
	if (!loaded_in_memory)
	{
		loaded_in_memory = true;
	
		// Vertices
		glGenBuffers(1, (GLuint*) &(mesh_data->id_vertices));
		if (mesh_data->id_vertices == 0)
		{
			DEBUG("[error] Vertices buffer has not been binded!");
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, mesh_data->id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh_data->num_vertices * 3, mesh_data->vertices, GL_STATIC_DRAW);
		}

		// Normals
		glGenBuffers(1, (GLuint*) &(mesh_data->id_normals));
		if (mesh_data->id_normals == 0)
		{
			DEBUG("[error] Normals buffer has not been binded!");
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, mesh_data->id_normals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh_data->num_normals * 3, mesh_data->normals, GL_STATIC_DRAW);
		}

		// Texture coordinates
		glGenBuffers(1, (GLuint*) &(mesh_data->id_tex_coord));
		if (mesh_data->id_tex_coord == 0)
		{
			DEBUG("[error] Texture coordinates buffer has not been binded!");
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, mesh_data->id_tex_coord);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh_data->num_tex_coord * 2, mesh_data->tex_coord, GL_STATIC_DRAW);
		}

		// Indices
		glGenBuffers(1, (GLuint*) &(mesh_data->id_indices));
		if (mesh_data->id_indices == 0)
		{
			DEBUG("[error] Indices buffer has not been binded!");
		}
		else
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data->id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*mesh_data->num_indices, mesh_data->indices, GL_STATIC_DRAW);
		}
	}

	return loaded_in_memory;
}

bool ResourceMesh::UnloadFromMemory()
{
	if (loaded_in_memory)
	{
		loaded_in_memory = false;

		glDeleteBuffers(1, (GLuint*)&mesh_data->id_indices);
		mesh_data->id_indices = 0;
		glDeleteBuffers(1, (GLuint*)&mesh_data->id_vertices);
		mesh_data->id_vertices = 0;
		glDeleteBuffers(1, (GLuint*)&mesh_data->id_tex_coord);
		mesh_data->id_tex_coord = 0;
		glDeleteBuffers(1, (GLuint*)&mesh_data->id_normals);
		mesh_data->id_normals = 0;
	}

	return !loaded_in_memory;
}

