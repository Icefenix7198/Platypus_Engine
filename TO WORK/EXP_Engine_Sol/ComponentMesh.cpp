#include "ComponentMesh.h"

#include "Application.h"

#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"


ComponentMesh::ComponentMesh()
{
	wireMode = false;
	drawVertexNormals = false;
	mesh = nullptr;
	Enable();
}

ComponentMesh::ComponentMesh(Mesh* _mesh)
{
	wireMode = false;
	drawVertexNormals = false;
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
	// To DO: Transformation
	if (drawVertexNormals)
	{
		DrawVertexNormals();
	}
	App->renderer3D->DrawMesh(mesh, wireMode);
	

	return true;
}

bool ComponentMesh::DrawVertexNormals()
{
	if(mesh != nullptr)
	{
		//glLineWidth(2.0f); //Not very good, not supported in a lot of systems values different from 1
		glBegin(GL_LINES);

		//glColor3b();
		GLfloat const color[3] = { (1.0 / 255), (240.0 / 255), (30.0 / 255) };
		glColor3fv(color); //Uses values from 1 to 0 no 255
		for (int i = 0; i <= mesh->num_vertex; i++)
		{
			glVertex3f(mesh->vertex[i*3], mesh->vertex[i*3 + 1], mesh->vertex[i*3 + 2]);
			glVertex3f(mesh->vertex[i*3] + mesh->normals[i*3], mesh->vertex[i*3 + 1] + mesh->normals[i*3 + 1], mesh->vertex[i*3 + 2] + mesh->normals[i*3 + 2]);
		}
		glEnd();
	}
	else
	{
		return false;
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
			ImGui::Checkbox("Draw Normals", &drawVertexNormals);
			
			ImGui::Text("Number vertex %d", mesh->num_vertex);
			ImGui::Text("Number normals %d", mesh->num_normals);
		}
		else
		{
			ImGui::Text("Empty Mesh");
		}
		
		
	}
}