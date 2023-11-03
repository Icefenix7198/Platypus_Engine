#include "ComponentMaterial.h"

#include "Application.h"

#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"

ComponentMaterial::ComponentMaterial()
{
	checkers = true;
}

ComponentMaterial::ComponentMaterial(GameObject* own)
{
	owner = own;
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::OnEditor()
{
	std::string butonChar;

	//Give an ID to each colapsing header to be able to have more than one of the same time
	//This must be done due to ImGui using the names as the ids of all menus and things
	int myPosInComponents = owner->GetComponentPosition(this);
	std::string idComponent;
	idComponent.append("##");
	idComponent.append(std::to_string(myPosInComponents).c_str());

	butonChar.clear();
	butonChar.append("Material");
	if (ImGui::CollapsingHeader(butonChar.append(idComponent).c_str()))
	{
		butonChar.clear();
		butonChar.append("##MaterialActive");
		ImGui::Checkbox(butonChar.append(idComponent).c_str(), &active); //El doble ## hace que no se muestre el texto. Es necesario poner un nombre distinto a cada checkbox y boton ya que ImGui usa el nombre como la ID

		ImGui::Text("Size:");
		ImGui::Text("Path:");

		butonChar.clear();
		butonChar.append("Checkers");
		if(ImGui::Checkbox(butonChar.append(idComponent).c_str(), &checkers))
		{
			activeCheck = true;
		}


	}
}

bool ComponentMaterial::Update()
{
	return false;
}




