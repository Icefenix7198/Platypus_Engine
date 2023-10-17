#include "ComponentTransform2D.h"

#include "GameObject.h"

#include "imgui\imgui.h"
#include "MathGeoLib\MathGeoLib.h"
//#include "openGL.h"

#include "ResourceMesh.h"
#include "Mesh.h"

#include <stack>

ComponentTransform2D::ComponentTransform2D() : Component()
{
	local_position.Set(0.0f, 0.0f, 0.0f);
	global_position.Set(0.0f, 0.0f, 0.0f);
	
	size.Set(0.0f, 0.0f);

	type = COMPONENT_TYPE::COMP_TRANSFORM_2D;
	name = "Rect Transform";

	AddResource(App->resource_manager->CreateNewResource(RESOURCE_TYPE::RES_MESHES, App->resource_manager->GenerateID(), 1));
	InitPanelMesh();
}

void ComponentTransform2D::InitPanelMesh()
{
	// Panel mesh: positions, size, vertices, indices
	panel->mesh_data = new Mesh();

	/* Order vertices:
	0 ----- 1
	|       |
	|       |
	2 ----- 3     */

	// Vertices
	panel->mesh_data->num_vertices = 4;
	panel->mesh_data->vertices = new math::float3[panel->mesh_data->num_vertices];

	ResizePanel();

	// Indices
	panel->mesh_data->num_indices = 6;
	panel->mesh_data->indices = new unsigned int[panel->mesh_data->num_indices];
	panel->mesh_data->indices[0] = 0;
	panel->mesh_data->indices[1] = 2;
	panel->mesh_data->indices[2] = 1;
	panel->mesh_data->indices[3] = 1;
	panel->mesh_data->indices[4] = 2;
	panel->mesh_data->indices[5] = 3;

	// Texture coordinates
	panel->mesh_data->num_tex_coord = 4;
	panel->mesh_data->tex_coord = new math::float2[panel->mesh_data->num_tex_coord];
	panel->mesh_data->tex_coord[0] = { 0.0f, 1.0f };
	panel->mesh_data->tex_coord[1] = { 1.0f, 1.0f };
	panel->mesh_data->tex_coord[2] = { 0.0f, 0.0f };
	panel->mesh_data->tex_coord[3] = { 1.0f, 0.0f };

	panel->LoadToMemory();
}

bool ComponentTransform2D::Update()
{
	if (apply_transformation)
	{
		RecalcTranslations();
		apply_transformation = false;
	}		

	return true;
}

void ComponentTransform2D::ShowEditorInfo()
{
	ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
	ImGui::Text(name);

	if (ImGui::DragFloat3("Position", &local_position.x, 1.0f, 0.0f, 0.0f, "%.3f"))
		apply_transformation = true;

	if (ImGui::DragFloat2("Size", &size.x, 1.0f, 1.0f, 1000.0f, "%.3f"))
		apply_transformation = true;

	ImGui::Separator();
}

void ComponentTransform2D::AddResource(Resource *res)
{
	res->IncrReferences();
	panel = (ResourceMesh*)res;
}

void ComponentTransform2D::RecalcTranslations()
{
	CalcGlobalTranslation();

	for (uint i = 0; i < game_object->children.size(); ++i)
		game_object->children[i]->transform_2d->RecalcTranslations();
}

void ComponentTransform2D::CalcGlobalTranslation()
{
	if (game_object->parent->transform_2d != nullptr)
		global_position = game_object->parent->transform_2d->GetGlobalPos() + local_position;
	else
		global_position = local_position;

	// Adapt UI panel to new dimensions
	ResizePanel();
}

void ComponentTransform2D::ResizePanel()
{
	panel->mesh_data->vertices[0] = global_position;
	panel->mesh_data->vertices[1] = global_position + math::float3(size.x, 0.0f, 0.0f);
	panel->mesh_data->vertices[2] = global_position + math::float3(0.0f, size.y, 0.0f);
	panel->mesh_data->vertices[3] = global_position + math::float3(size, 0.0f);
	
	glBindBuffer(GL_ARRAY_BUFFER, panel->mesh_data->id_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, 0, panel->mesh_data->num_vertices * sizeof(math::float3), panel->mesh_data->vertices);
}

void ComponentTransform2D::Move(const math::vec &movement)
{
	local_position += movement;
	apply_transformation = true;
}

bool ComponentTransform2D::ContainsPoint(const math::float2 &point) const
{
	if (point.x < global_position.x || point.x > global_position.x + size.x || point.y < global_position.y || point.y > global_position.y + size.y)
		return false;
	return true;
}

const math::vec &ComponentTransform2D::GetLocalPos() const
{
	return local_position;
}

const math::vec &ComponentTransform2D::GetGlobalPos() const
{
	return global_position;
}

void ComponentTransform2D::SetLocalPos(const math::vec &local_pos)
{
	local_position = local_pos;
	apply_transformation = true;
}

void ComponentTransform2D::SetSize(const math::float2 &size)
{
	this->size = size;
	apply_transformation = true;
}

bool ComponentTransform2D::Save(JSONParser &go)
{
	/*JSONParser comp_trans;

	comp_trans.AddInt("Type", type);

	comp_trans.AddPoints("Local position", local_position, 3);
	comp_trans.AddPoints("Local scale", local_scale, 3);
	comp_trans.AddPoints("Local rotation", local_rotation_quat, 4);

	comp_trans.AddPoints("Parent transform", parent_transform, 16);

	comp_trans.AddPoints("Forward", forward, 3);
	comp_trans.AddPoints("Left", left, 3);
	comp_trans.AddPoints("Up", up, 3);

	go.AddArray(comp_trans);*/

	return true;
}

bool ComponentTransform2D::Load(JSONParser &comp)
{
	//comp.GetPoints("Local position", local_position, 3);
	//comp.GetPoints("Local scale", local_scale, 3);
	//comp.GetPoints("Local rotation", local_rotation_quat, 4);

	//comp.GetPoints("Parent transform", parent_transform, 16);

	//comp.GetPoints("Forward", forward, 3);
	//comp.GetPoints("Left", left, 3);
	//comp.GetPoints("Up", up, 3);

	//CalcWorldTransformMatrix();

	//// Inspector will show Euler representation on degrees to be more understandable.
	//local_rotation_euler_deg.x = math::RadToDeg(local_rotation_euler_rad.x);
	//local_rotation_euler_deg.y = math::RadToDeg(local_rotation_euler_rad.y);
	//local_rotation_euler_deg.z = math::RadToDeg(local_rotation_euler_rad.z);

	return true;
}


