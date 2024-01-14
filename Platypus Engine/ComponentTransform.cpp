#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h" //Necesary for AABBs
#include "GameObject.h"

#include "MathGeoLib/include/Math/Quat.h"

#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"

ComponentTransform::ComponentTransform()
{
	translation.x = 0;
	translation.y = 0;
	translation.z = 0;
	pos = {translation.x,translation.y,translation.z};

	scaling.x = 1;
	scaling.y = 1;
	scaling.z = 1;
	scale = { scaling.x, scaling.y, scaling.z };

	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;
	rotation.w = 0;
	rot = {rotation.x, rotation.y, rotation.z, rotation.w};

	Enable();

	RecalculateMatrix();
}

ComponentTransform::ComponentTransform(GameObject* own)
{
	owner = own;
	UUID = App->resources->GenerateNewUID();

	translation.x = 0;
	translation.y = 0;
	translation.z = 0;
	pos = { translation.x,translation.y,translation.z };

	scaling.x = 1;
	scaling.y = 1;
	scaling.z = 1;
	scale = { scaling.x, scaling.y, scaling.z };

	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;
	rotation.w = 0;
	rot = { rotation.x, rotation.y, rotation.z, rotation.w };

	Enable();

	RecalculateMatrix();
}

ComponentTransform::ComponentTransform(aiVector3D vecPos, aiVector3D vecScale, aiQuaternion quatRot)
{
	translation = vecPos;
	pos = { translation.x,translation.y,translation.z };

	scaling = vecScale;
	scale = { scaling.x, scaling.y, scaling.z };

	rotation = quatRot;
	rot = { rotation.x, rotation.y, rotation.z, rotation.w };

	RecalculateMatrix(); //Also makes the local matrix

	Enable();
}

ComponentTransform::~ComponentTransform()
{
}

bool ComponentTransform::GetActive()
{
	return active;
}

void ComponentTransform::SetValues(aiVector3D translation, aiVector3D scaling, aiQuaternion rotation)
{
	pos = { translation.x,translation.y,translation.z };

	scale = { scaling.x, scaling.y, scaling.z };

	rot = { rotation.x, rotation.y, rotation.z, rotation.w };
	
	RecalculateMatrix();
}

void ComponentTransform::GenerateLocalMatrix()
{
	localTransform = float4x4::FromTRS(pos, rot, scale);
}

void ComponentTransform::GenerateGlobalMatrix(bool regenerateLocal)
{
	//The operation depends of this, so just in case it doesn't exist yet (but with reparent we don't want that executed
	if (regenerateLocal) {	GenerateLocalMatrix();	}

	if (owner->parent == nullptr) //Si es el root su global es igual a su loca que es 0,0,0 en todo menos scale que es 1,1,1
	{
		globalTransform = localTransform; //Global transform of root is global transform
	}
	else
	{
		globalTransform = owner->parent->objTransform->globalTransform * localTransform;
	}
}

float4x4 ComponentTransform::CreateMatrix(float3 translation, float3 scaling, Quat rotation)
{
	//Si entendi bien este tutorial https://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
	
	float4x4 m;
	m = m.identity;
	m = float4x4::FromTRS(translation, rotation, scaling); //Creates automatically the matrix, thanks math library

	return m; //Translate*rotate*scale
}

bool ComponentTransform::RecalculateMatrix(bool regenerateLocal)
{
	GenerateGlobalMatrix(regenerateLocal);
	
	if (!owner->children.empty())
	{
		for (int i = 0; i < owner->children.size(); i++)
		{
			owner->children.at(i)->objTransform->RecalculateMatrix(regenerateLocal);
		}
	}
	ComponentMesh* mesh = (ComponentMesh*)owner->GetComponentByType(ComponentType::MESH);
	if (mesh != nullptr)
	{
		mesh->GetGlobalAABB();
	}
	

	return true;
}

float4x4 ComponentTransform::ReparentLocalMatrix(GameObject* newParent)
{
	float4x4 newLocal = newLocal.identity;
	float4x4 parentGlobal = newParent->objTransform->localTransform;
	parentGlobal.Inverse();
	newLocal = parentGlobal * globalTransform;
	localTransform = newLocal;
	newLocal.Decompose(pos, rot, scale);
	return newLocal;
}

void ComponentTransform::OnEditor()
{
	if(ImGui::CollapsingHeader("Transform"))
	{

		ImGui::Checkbox("##Transform", &active);
		
		if (ImGui::DragFloat3("Position", &pos[0],0.1f)) 
		{
			RecalculateMatrix();
		};

		angles = rot.ToEulerXYZ();
		angles.x *= RADTODEG; angles.y *= RADTODEG; angles.z *= RADTODEG;
		
		if (ImGui::DragFloat3("Rotation", &angles[0],0.1f))
		{
			rot = Quat::FromEulerXYZ(DEGTORAD * angles.x, DEGTORAD * angles.y, DEGTORAD * angles.z); //TODO: Por algun motivo esta formula me los divide entre 2, porque? NPI
			rot = rot.Normalized();

			RecalculateMatrix();
		};

		if (ImGui::DragFloat3("Scale", &scale[0],0.1f))
		{
			RecalculateMatrix();
		};
	}
}




