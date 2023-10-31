#include "ComponentMesh.h"

#include "Application.h"

#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"


ComponentMesh::ComponentMesh()
{
	wireMode = false;
	mesh = nullptr;
	Enable();
}

ComponentMesh::ComponentMesh(Mesh* _mesh)
{
	mesh = _mesh;
	Enable();
}



ComponentMesh::~ComponentMesh()
{
	if (mesh != nullptr)
	{
		delete mesh;
	}
}

bool ComponentMesh::Update()
{
	//Draw mesh

	if (App->renderer3D->GetDrawingMode() || wireMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	return true;
}

void ComponentMesh::OnEditor()
{

	if (ImGui::CollapsingHeader("Mesh"))
	{
		
		ImGui::Checkbox("##Mesh", &active); //El doble ## hace que no se muestre el texto. Es necesario poner un nombre distinto a cada checkbox y boton ya que ImGui usa el nombre como la ID
		ImGui::SameLine();
		if(ImGui::Button("Delete component"))
		{
			this->~ComponentMesh();
		}
		if(mesh != nullptr)
		{
			ImGui::Text("Number vertex %d", mesh->num_vertex);
			ImGui::Text("Number normals %d", mesh->num_normals);
		}
		else
		{
			ImGui::Text("Empty Mesh");
		}
		
		
	}
}