#include "ComponentMesh.h"

#include "Application.h"

#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"


ComponentMesh::ComponentMesh()
{
	wireMode = false;
}

ComponentMesh::ComponentMesh(Mesh* _mesh)
{
	mesh = _mesh;
}



ComponentMesh::~ComponentMesh()
{
	delete mesh;
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

	if (ImGui::CollapsingHeader("Transform"))
	{
		
	}
}