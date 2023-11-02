#include "ComponentMesh.h"

#include "Application.h"

#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"


ComponentMesh::ComponentMesh()
{
	wireMode = false;
	drawVertexNormals = drawFaceNormals = false;
	mesh = nullptr;
	Enable();
}

ComponentMesh::ComponentMesh(Mesh* _mesh)
{
	wireMode = false;
	drawVertexNormals = drawFaceNormals = false;
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
	if(mesh!=nullptr && active)
	{
		if (drawVertexNormals) {	DrawVertexNormals();}
		if (drawFaceNormals) { DrawFaceNormals(); }
		App->renderer3D->DrawMesh(mesh, wireMode);
	}
	
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
		GLfloat const color2[3] = { (1.0f), (1.0f), (1.0f) };
		glColor3fv(color2); //Return to White as normal
	}
	else
	{
		return false;
	}
	
	return true;
}

bool ComponentMesh::DrawFaceNormals()
{
	//float* facesCenter = new float[mesh->num_index/3]; //Num faces

	glBegin(GL_LINES);
	GLfloat const color[3] = { (200.0 / 255), (10.0 / 255), (200.0 / 255) };
	glColor3fv(color); //Uses values from 1 to 0 no 255
	for (uint i = 0; i < mesh->num_index; i += 3)
	{
		//Becose each vertex has 3 components we must take steps of three to select each vertex of the array
		uint index1 = mesh->index[i]*3 ; 
		uint index2 = mesh->index[i + 1]*3 ;
		uint index3 = mesh->index[i + 2]*3 ;

		vec3 vertex0(mesh->vertex[index1], mesh->vertex[index1 + 1], mesh->vertex[index1 + 2]); //First vertex of the face
		vec3 vertex1(mesh->vertex[index2], mesh->vertex[index2 + 1], mesh->vertex[index2 + 2]); //Second vertex of the face
		vec3 vertex2(mesh->vertex[index3], mesh->vertex[index3 + 1], mesh->vertex[index3 + 2]); //Third vertex of the face

		//We take 2 vector of the dots of the mesh becose cross product gives us the perpendicular direction AKA the normal of the face
		vec3 vec0 = vertex0 - vertex2;
		vec3 vec1 = vertex1 - vertex2;
		vec3 normalVec = cross(vec0, vec1);

		vec3 normalizedVec = normalize(normalVec); //We normalize to reduce the size

		//vec3 faceCenter = { (((vertex0.x + vertex1.x + vertex2.x) / 3),((vertex0.y + vertex1.y + vertex2.y) / 3),((vertex0.z + vertex1.z + vertex2.z) / 3)) }; //X,Y,Z of the center of face
		float faceCenters[3] ; //X,Y,Z of the center of face
		faceCenters[0] = (vertex0.x + vertex1.x + vertex2.x)/3;
		faceCenters[1] = (vertex0.y + vertex1.y + vertex2.y)/3;
		faceCenters[2] = (vertex0.z + vertex1.z + vertex2.z)/3;
		
		glVertex3f(faceCenters[0],faceCenters[1],faceCenters[2]);
		glVertex3f(faceCenters[0] + normalVec.x, faceCenters[1] + normalVec.y, faceCenters[2] + normalVec.z);
		
	}

	glEnd();
	GLfloat const color2[3] = { (1.0f), (1.0f), (1.0f) };
	glColor3fv(color2); //Return to White as normal

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
			ImGui::Checkbox("Draw Wireframe Mode ##Component", &wireMode);
			ImGui::Checkbox("Draw Vertex Normals", &drawVertexNormals);
			ImGui::Checkbox("Draw Face Normals", &drawFaceNormals);
			
			ImGui::Text("Number vertex %d", mesh->num_vertex);
			ImGui::Text("Number normals %d", mesh->num_normals);
		}
		else
		{
			ImGui::Text("Empty Mesh");
		}
		
		
	}
}