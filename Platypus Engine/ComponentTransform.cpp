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

	GenerateGlobalMatrix();
}

ComponentTransform::ComponentTransform(GameObject* own)
{
	owner = own;

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

	GenerateGlobalMatrix();
}

ComponentTransform::ComponentTransform(aiVector3D vecPos, aiVector3D vecScale, aiQuaternion quatRot)
{
	translation = vecPos;
	pos = { translation.x,translation.y,translation.z };

	scaling = vecScale;
	scale = { scaling.x, scaling.y, scaling.z };

	rotation = quatRot;
	rot = { rotation.x, rotation.y, rotation.z, rotation.w };

	GenerateGlobalMatrix(); //Also makes the local matrix

	Enable();
}

ComponentTransform::~ComponentTransform()
{
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
	//Generate AABB if has component mesh (we do it here)
	ComponentMesh* mesh = (ComponentMesh*)owner->GetComponentByType(ComponentType::MESH);
	if (mesh != nullptr)
	{
		mesh->GenerateLocalAABB();
	}

	rot = Quat::FromEulerXYZ(rot.x, rot.y, rot.z);
	rot.Normalize();

	localTransform = CreateMatrix(pos,scale, rot );
}

void ComponentTransform::GenerateGlobalMatrix()
{
	//The operation depends of this, so just in case it doesn't exist yet 
	GenerateLocalMatrix();

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

bool ComponentTransform::RecalculateMatrix()
{
	GenerateGlobalMatrix();
	ComponentMesh* mesh = (ComponentMesh*)owner->GetComponentByType(ComponentType::MESH);
	if (mesh != nullptr)
	{
		mesh->GetGlobalAABB();
	}
	if (!owner->children.empty())
	{
		for (int i = 0; i < owner->children.size(); i++)
		{
			owner->children.at(i)->objTransform->RecalculateMatrix();
		}
	}
	

	return true;
}

void ComponentTransform::OnEditor()
{
	if(ImGui::CollapsingHeader("Transform"))
	{

		ImGui::Checkbox("##Transform", &active);
		
		float posi[3] = { pos.x, pos.y, pos.z };
		if (ImGui::DragFloat3("Position", posi)) 
		{
			pos.x = posi[0]; pos.y = posi[1]; pos.z = posi[2];
			RecalculateMatrix();
		};
		float rota[3] = { RADTODEG * (rot.x), RADTODEG * (rot.y), RADTODEG * (rot.z) };//Values given in Radians,must translate to degrees
		if (ImGui::DragFloat3("Rotation", rota))
		{
			rot.x = rota[0] * DEGTORAD; rot.y = rota[1] * DEGTORAD; rot.z = rota[2] * DEGTORAD;
			RecalculateMatrix();
		};
		float esca[3] = { scale.x, scale.y, scale.z };
		if (ImGui::DragFloat3("Scale", esca))
		{
			scale.x = esca[0]; scale.y = esca[1]; scale.z = esca[2];
			RecalculateMatrix();
		};
	}
}




