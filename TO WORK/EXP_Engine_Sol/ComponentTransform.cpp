#include "ComponentTransform.h"

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
}

ComponentTransform::ComponentTransform(aiVector3D vecPos)
{
	translation.x = vecPos.x;
	translation.y = vecPos.y;
	translation.z = vecPos.z;
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
}

ComponentTransform::ComponentTransform(aiVector3D vecPos, aiVector3D vecScale, aiQuaternion quatRot)
{
	translation = vecPos;
	pos = { translation.x,translation.y,translation.z };

	scaling = vecScale;
	scale = { scaling.x, scaling.y, scaling.z };

	rotation = quatRot;
	rot = { rotation.x, rotation.y, rotation.z, rotation.w };
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::OnEditor()
{
	
	if(ImGui::CollapsingHeader("Transform"))
	{
		//LOG("OnEditor de Transform")
		ImGui::AlignTextToFramePadding();
		//Position
		ImGui::InputFloat("X", &pos.x, 0.5F, 0.0F, "%.5f"); ImGui::SameLine();
		ImGui::InputFloat("Y", &pos.y, 0.0F, 0.0F, "%.5f"); ImGui::SameLine();
		ImGui::InputFloat("Z", &pos.z, 0.0F, 0.0F, "%.5f"); ImGui::SameLine();
		ImGui::Text("Position");

		//Rotation
		ImGui::Text("X"); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine();
		ImGui::Text("Rotation");

		//Scale
		ImGui::Text("X"); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine();
		ImGui::Text("Scale");
		//ImGui::End();

		//ImGui::BeginTable()
	}
}


