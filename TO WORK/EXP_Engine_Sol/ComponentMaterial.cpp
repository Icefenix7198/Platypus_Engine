#include "ComponentMaterial.h"

#include "Application.h"

#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"

ComponentMaterial::ComponentMaterial()
{
	checkers = true;
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::OnEditor()
{
	if (ImGui::CollapsingHeader("Material"))
	{

		ImGui::Checkbox("##Material", &active); //El doble ## hace que no se muestre el texto. Es necesario poner un nombre distinto a cada checkbox y boton ya que ImGui usa el nombre como la ID

		ImGui::Text("Size:");
		ImGui::Text("Path:");

		if(ImGui::Checkbox("Checkers", &checkers))
		{
			activeCheck = true;
		}


	}
}

bool ComponentMaterial::Update()
{
	return false;
}




