#include "ComponentMesh.h"

#include "Application.h"

#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"


ComponentMesh::ComponentMesh()
{
	wireMode = false;
	drawNormals = false;
	mesh = nullptr;
	Enable();
}

ComponentMesh::ComponentMesh(Mesh* _mesh)
{
	wireMode = false;
	drawNormals = false;
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

bool ComponentMesh::DrawNormals()
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
			//LOG("Executed %d times", i);
			//float normalizer = sqrt(Pow((float)mesh->normals[i*3], 2) + Pow((float)mesh->normals[i*3 + 1], 2) + Pow((float)mesh->normals[i*3 + 2], 2)); //Normals vector normalized (at the moment is huuuge so we need to normalize it
			glVertex3f(mesh->vertex[i*3], mesh->vertex[i*3 + 1], mesh->vertex[i*3 + 2]);
			glVertex3f(mesh->vertex[i*3] + mesh->normals[i*3]/*/ normalizer*/, mesh->vertex[i*3 + 1] + mesh->normals[i*3 + 1]/*/normalizer*/, mesh->vertex[i*3 + 2] + mesh->normals[i*3 + 2]/*/ normalizer*/);
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
			ImGui::Checkbox("Draw Normals", &drawNormals);
			if (drawNormals) //TEMPORAL PARA TESTING, TENDRA QUE IR EN EL UPDATE
			{
				DrawNormals();
			}
			ImGui::Text("Number vertex %d", mesh->num_vertex);
			ImGui::Text("Number normals %d", mesh->num_normals);
		}
		else
		{
			ImGui::Text("Empty Mesh");
		}
		
		
	}
}