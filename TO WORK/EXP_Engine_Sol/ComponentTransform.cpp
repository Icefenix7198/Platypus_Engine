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

	Enable();
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

	Enable();
}

ComponentTransform::ComponentTransform(aiVector3D vecPos, aiVector3D vecScale, aiQuaternion quatRot)
{
	translation = vecPos;
	pos = { translation.x,translation.y,translation.z };

	scaling = vecScale;
	scale = { scaling.x, scaling.y, scaling.z };

	rotation = quatRot;
	rot = { rotation.x, rotation.y, rotation.z, rotation.w };

	Enable();
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::OnEditor()
{
	
	if(ImGui::CollapsingHeader("Transform"))
	{

		ImGui::Checkbox("##Transform", &active);
		

		//Each table goes apart to avoid modifing non desired values
		if (ImGui::BeginTable("Position", 4, 0)) 
		{
			//(active) ? 0 :ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly --> When active= false los valores no se pueden modificar desde aqui
			// 
			// 
			//Position
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::InputFloat("X", &pos.x, 0.0F, 0.0F, "%.5f", (active) ? 0 :ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly); 
			ImGui::TableNextColumn(); ImGui::InputFloat("Y", &pos.y, 0.0F, 0.0F, "%.5f", (active) ? 0 : ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
			ImGui::TableNextColumn(); ImGui::InputFloat("Z", &pos.z, 0.0F, 0.0F, "%.5f", (active) ? 0 : ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
			ImGui::TableNextColumn();ImGui::Text("Position");

			ImGui::EndTable();
		}
		if (ImGui::BeginTable("Position", 4, 0))
		{
			//Rotation
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::InputFloat("X", &rot.x, 0.0F, 0.0F, "%.2f", (active) ? 0 : ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
			ImGui::TableNextColumn(); ImGui::InputFloat("Y", &rot.y, 0.0F, 0.0F, "%.2f", (active) ? 0 : ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
			ImGui::TableNextColumn(); ImGui::InputFloat("Z", &rot.z, 0.0F, 0.0F, "%.2f", (active) ? 0 : ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
			ImGui::TableNextColumn(); ImGui::Text("Rotation");

			ImGui::EndTable();
		}
		if (ImGui::BeginTable("Position", 4, 0))
		{
			//Scale
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::InputFloat("X", &scale.x, 0.0F, 0.0F, "%.2f", (active) ? 0 : ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
			ImGui::TableNextColumn(); ImGui::InputFloat("Y", &scale.y, 0.0F, 0.0F, "%.2f", (active) ? 0 : ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
			ImGui::TableNextColumn(); ImGui::InputFloat("Z", &scale.z, 0.0F, 0.0F, "%.2f", (active) ? 0 : ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
			ImGui::TableNextColumn(); ImGui::Text("Scale");

			ImGui::EndTable();
		}
	}
}

void ComponentTransform::SetValues(aiVector3D translation, aiVector3D scaling, aiQuaternion rotation)
{
	pos = { translation.x,translation.y,translation.z };

	scale = { scaling.x, scaling.y, scaling.z };

	rot = { rotation.x, rotation.y, rotation.z, rotation.w };
}


