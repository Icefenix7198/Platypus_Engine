#include "ComponentTransform.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"

#include "Assimp\include\scene.h"
#include "imgui\imgui.h"

#include <stack>

ComponentTransform::ComponentTransform() : Component()
{
	forward_dir.Set(0.0f, 0.0f, 1.0f);
	left_dir.Set(1.0f, 0.0f, 0.0f);
	up_dir.Set(0.0f, 1.0f, 0.0f);

	world_transform.SetIdentity();
	local_transform.SetIdentity();
	parent_transform.SetIdentity();

	local_position.Set(0.0f, 0.0f, 0.0f);
	local_scale.Set(1.0f, 1.0f, 1.0f);

	local_rotation_euler_rad.Set(0.0f, 0.0f, 0.0f);
	local_rotation_euler_deg.Set(0.0f, 0.0f, 0.0f);
	local_rotation_quat.Set(0.0f, 0.0f, 0.0f, 1.0f);

	type = COMPONENT_TYPE::COMP_TRANSFORM;
	name = "Transform";
}

bool ComponentTransform::Update()
{
	if (apply_transformation)
	{
		RecalcTransformations();
		apply_transformation = false;
	}		
		
	return true;
}

void ComponentTransform::ShowEditorInfo()
{
	ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
	ImGui::Text(name);	

	if (ImGui::DragFloat3("Position", &local_position.x, 0.25f, 0.0f, 0.0f, "%.3f"))
		apply_transformation = true;
		
	if (ImGui::DragFloat3("Rotation", &local_rotation_euler_deg.x, 0.25f, 0.0f, 0.0f, "%.3f"))
	{
		apply_transformation = true;
		EulerToQuat(local_rotation_euler_deg, local_rotation_quat);
	}

	if (ImGui::DragFloat3("Scale", &local_scale.x, 0.25f, 0.0f, 0.0f, "%.3f"))
		apply_transformation = true;

	if (ImGui::TreeNode("Transform matrix"))
	{
		ImGui::InputFloat4("", &world_transform[0][0], 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat4("", &world_transform[1][0], 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat4("", &world_transform[2][0], 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat4("", &world_transform[3][0], 3, ImGuiInputTextFlags_ReadOnly);
		ImGui::TreePop();
	}

	ImGui::Separator();
}

void ComponentTransform::SetComponent(aiNode *go)
{
	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion rotating;

	// Creating local matrix transformation
	go->mTransformation.Decompose(scaling, rotating, translation);

	local_position = { translation.x, translation.y, translation.z };
	local_scale = { scaling.x, scaling.y, scaling.z };
	local_rotation_quat = { rotating.x, rotating.y, rotating.z, rotating.w };

	// Creating parent matrix transformation. All parent transformations of this GameObject will be combined into one
	math::float3 parent_position;
	math::float3 parent_scale;
	math::Quat parent_rotation;

	QuatToEuler(local_rotation_quat, local_rotation_euler_rad);

	// Inspector will show Euler representation on degrees to be more understandable.
	local_rotation_euler_deg.x = math::RadToDeg(local_rotation_euler_rad.x);
	local_rotation_euler_deg.y = math::RadToDeg(local_rotation_euler_rad.y);
	local_rotation_euler_deg.z = math::RadToDeg(local_rotation_euler_rad.z);

	aiNode *node = go->mParent;
	while (node != nullptr)
	{
		node->mTransformation.Decompose(scaling, rotating, translation);
		parent_position = { translation.x, translation.y, translation.z };
		parent_scale = { scaling.x, scaling.y, scaling.z };
		parent_rotation = { rotating.x, rotating.y, rotating.z, rotating.w };

		math::float4x4 parent_matrix = CalcTransformMatrix(parent_position, parent_scale, parent_rotation);
		parent_transform = parent_matrix * parent_transform;
		node = node->mParent;
	}

	CalcWorldTransformMatrix();
}

// CalcWorldTransformMatrix recalculates both parent and local transformation for the current gameobject. Besides, the method recursively
// recalculates the world matrix transformation for all its children.
void ComponentTransform::RecalcTransformations()
{
	CalcWorldTransformMatrix();

	for (uint i = 0; i < game_object->children.size(); ++i)
		game_object->children[i]->transform->RecalcTransformations();
}

void ComponentTransform::CalcWorldTransformMatrix()
{
	local_transform = CalcTransformMatrix(local_position, local_scale, local_rotation_quat);

	if (game_object->parent->transform != nullptr)
	{
		parent_transform = game_object->parent->transform->world_transform;
		world_transform = parent_transform * local_transform;
	}
	else
		world_transform = local_transform;

	forward = (world_transform.RotatePart() * forward_dir).Normalized();
	left = (world_transform.RotatePart() * left_dir).Normalized();
	up = (world_transform.RotatePart() * up_dir).Normalized();

	// Updating frustum and AABB
	ComponentMesh *comp = (ComponentMesh*)game_object->GetComponentByType(COMPONENT_TYPE::COMP_MESH);
	ComponentCamera *camera = (ComponentCamera*)game_object->GetComponentByType(COMPONENT_TYPE::COMP_CAMERA);

	if (comp != nullptr) comp->ApplyTransformToAABB();
	if (camera != nullptr) camera->ApplyTransformToFrustum();
}

math::float4x4 ComponentTransform::CalcTransformMatrix(const math::float3 &pos, const math::float3 &scale, const math::Quat &rot)
{
	math::float4x4 mat = math::float4x4::FromTRS(pos, rot, scale);
	return mat;
}

void ComponentTransform::QuatToEuler(math::Quat &quat, math::float3 &out_euler)
{
	// I cannot properly implement MathGeoLib methods in order to make that transformation, 
	// (Quat->Euler, Euler->Quat), so basic theory has been used.

	float ysqr = quat.y * quat.y;
	float t0 = -2.0f * (ysqr + quat.z * quat.z) + 1.0f;
	float t1 = +2.0f * (quat.x * quat.y + quat.w * quat.z);
	float t2 = -2.0f * (quat.x * quat.z - quat.w * quat.y);
	float t3 = +2.0f * (quat.y * quat.z + quat.w * quat.x);
	float t4 = -2.0f * (quat.x * quat.x + ysqr) + 1.0f;

	t2 = t2 > 1.0f ? 1.0f : t2;
	t2 = t2 < -1.0f ? -1.0f : t2;

	out_euler.y = math::Asin(t2);			// Pitch
	out_euler.x = math::Atan2(t3, t4);		// Roll
	out_euler.z = math::Atan2(t1, t0);		// Yaw

	//Roll  rotation about the X - axis
	//Pitch  rotation about the Y - axis
	//Yaw rotation about the Z - axis*/
}

void ComponentTransform::EulerToQuat(math::float3 &euler, math::Quat &out_quat)
{
	local_rotation_euler_rad.x = math::DegToRad(euler.x);
	local_rotation_euler_rad.y = math::DegToRad(euler.y);
	local_rotation_euler_rad.z = math::DegToRad(euler.z);

	float t0 = math::Cos(local_rotation_euler_rad.z * 0.5f); // Yaw
	float t1 = math::Sin(local_rotation_euler_rad.z * 0.5f);
	float t2 = math::Cos(local_rotation_euler_rad.x * 0.5f); // Roll
	float t3 = math::Sin(local_rotation_euler_rad.x * 0.5f);
	float t4 = math::Cos(local_rotation_euler_rad.y * 0.5f); // Pitch
	float t5 = math::Sin(local_rotation_euler_rad.y * 0.5f);

	out_quat.w = t0 * t2 * t4 + t1 * t3 * t5;
	out_quat.x = t0 * t3 * t4 - t1 * t2 * t5;
	out_quat.y = t0 * t2 * t5 + t1 * t3 * t4;
	out_quat.z = t1 * t2 * t4 - t0 * t3 * t5;

	out_quat.Normalize();
}

void ComponentTransform::Move(const math::vec &movement)
{
	local_position += movement;
	apply_transformation = true;
}

void ComponentTransform::SetPos(const math::vec &pos)
{
	local_position = pos;
	apply_transformation = true;
}

void ComponentTransform::RotateAngleAxis(float angle_rad, const math::vec &axis)
{
	math::Quat rot_quat = math::Quat::RotateAxisAngle(axis, angle_rad);
	local_rotation_quat = local_rotation_quat.Mul(rot_quat);
	apply_transformation = true;

	QuatToEuler(local_rotation_quat, local_rotation_euler_rad);
	local_rotation_euler_deg.x = math::RadToDeg(local_rotation_euler_rad.x);
	local_rotation_euler_deg.y = math::RadToDeg(local_rotation_euler_rad.y);
	local_rotation_euler_deg.z = math::RadToDeg(local_rotation_euler_rad.z);
}

const math::vec &ComponentTransform::GetPos() const
{
	return local_position;
}

bool ComponentTransform::Save(JSONParser &go)
{
	JSONParser comp_trans;

	comp_trans.AddInt("Type", type);

	comp_trans.AddPoints("Local position", local_position, 3);
	comp_trans.AddPoints("Local scale", local_scale, 3);
	comp_trans.AddPoints("Local rotation", local_rotation_quat, 4);

	comp_trans.AddPoints("Parent transform", parent_transform, 16);

	comp_trans.AddPoints("Forward", forward, 3);
	comp_trans.AddPoints("Left", left, 3);
	comp_trans.AddPoints("Up", up, 3);

	go.AddArray(comp_trans);

	return true;
}

bool ComponentTransform::Load(JSONParser &comp)
{
	comp.GetPoints("Local position", local_position, 3);
	comp.GetPoints("Local scale", local_scale, 3);
	comp.GetPoints("Local rotation", local_rotation_quat, 4);

	comp.GetPoints("Parent transform", parent_transform, 16);

	comp.GetPoints("Forward", forward, 3);
	comp.GetPoints("Left", left, 3);
	comp.GetPoints("Up", up, 3);

	CalcWorldTransformMatrix();

	// Inspector will show Euler representation on degrees to be more understandable.
	local_rotation_euler_deg.x = math::RadToDeg(local_rotation_euler_rad.x);
	local_rotation_euler_deg.y = math::RadToDeg(local_rotation_euler_rad.y);
	local_rotation_euler_deg.z = math::RadToDeg(local_rotation_euler_rad.z);

	return true;
}


