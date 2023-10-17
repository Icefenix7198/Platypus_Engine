#include "ComponentMesh.h"

#include "Globals.h"
#include "GameObject.h"
#include "ComponentTransform.h"

#include "ResourceMesh.h"
#include "Mesh.h"

#include "Assimp\include\mesh.h"
#include "imgui\imgui.h"

ComponentMesh::ComponentMesh() : Component()
{
	wire = false;

	bounding_box.SetNegativeInfinity();
	initial_bounding_box.SetNegativeInfinity();

	type = COMPONENT_TYPE::COMP_MESH;
	name = "Mesh";

	resource = nullptr;
}

ComponentMesh::~ComponentMesh()
{ 
	if (resource->mesh_data != nullptr)
	{
		resource->DecrReferences();

		if (resource->GetNumReferences() == 0)
		{
			resource->UnloadFromMemory();
			delete resource->mesh_data;
			resource->mesh_data = nullptr;
		}		
	}		
}

void ComponentMesh::AddResource(Resource *res)
{
	res->IncrReferences();
	resource = (ResourceMesh*)res;	

	// AABB
	initial_bounding_box.Enclose((math::float3*) resource->mesh_data->vertices, resource->mesh_data->num_vertices);
	ApplyTransformToAABB();
}

void ComponentMesh::ShowEditorInfo()
{
	ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
	ImGui::Text(name);

	ImGui::Checkbox("Active##Mesh", &active);
	ImGui::Checkbox("Wireframe##Mesh", &wire);
	ImGui::Text("Number of vertices: %d", resource->mesh_data->num_vertices);
	ImGui::Text("Number of normals: %d", resource->mesh_data->num_normals);
	ImGui::Text("Number of texture coordinates: %d", resource->mesh_data->num_tex_coord);
	ImGui::Text("Number of indices: %d", resource->mesh_data->num_indices);

	ImGui::Separator();
}

void ComponentMesh::ApplyTransformToAABB()
{
	math::OBB tmp = initial_bounding_box.Transform(game_object->transform->world_transform);
	bounding_box.SetNegativeInfinity();
	bounding_box.Enclose(tmp);
}

bool ComponentMesh::Save(JSONParser &go)
{
	JSONParser comp_mesh;

	comp_mesh.AddInt("Type", type);
	comp_mesh.AddBoolean("Wire", wire);
	comp_mesh.AddUUID("Resource ID", resource->id);
	comp_mesh.AddBoolean("Active", active);

	go.AddArray(comp_mesh);

	return true;
}

bool ComponentMesh::Load(JSONParser &comp)
{
	AddResource(App->resource_manager->Get(comp.GetUUID("Resource ID")));
	wire = comp.GetBoolean("Wire");	
	active = comp.GetBoolean("Active");

	return true;
}