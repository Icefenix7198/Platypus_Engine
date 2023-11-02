#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_sdl2.h"
#include "ImGui/backends/imgui_impl_opengl3.h"

#include "aasimp.h"

#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"


#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")

#ifdef _DEBUG
#pragma comment (lib, "MathGeoLib/libx86/Debug/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#else
#pragma comment (lib, "MathGeoLib/libx86/Release/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#endif // _DEBUG


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		LOG("OpenGL context correctly created");
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(App->window->GetVSYNC() && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		else
		{
			LOG("OpenGL Projection correctly created");
		}
		

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		else
		{
			LOG("OpenGL Modelview correctly created");
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		else
		{
			LOG("OpenGL clear color correctly created");
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);

		glewInit();
	}

	// Projection matrix for
	OnResize(App->window->GetScreenWidth(), App->window->GetScreenHeigth());

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	if(nullptr !=ImGui::CreateContext())
	{
		LOG("ImGui context correctly created");
	}
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; //@ANDREU This flag causes errors with the coment: 
	// Forgot to call UpdatePlatformWindows() in main loop after EndFrame()? Check examples/ applications for reference."

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	if(ImGui_ImplSDL2_InitForOpenGL(App->window->window, context))
	{
		LOG("SDL2 for OpenGL correctly initializated");
	}
	if(ImGui_ImplOpenGL3_Init("#version 130"))
	{
		LOG("ImGui version 130 for OpenGL3 correctly initializated");
	}

	Grid.axis = true;

	//Load Baker House
	aasimp::Load("Assets/3DObjects/baker_house/BakerHouse.fbx"); //It must be done in Renderer because OpenGL isn't inizialitzed in scene.

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	Checkers(); //Init checkers for textures

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	//Functions to draw in direct mode here (it will need to go away)
	//DrawCubeDirectMode();

	
	DrawAllMeshes();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	App->editor->DrawEditor();
	
	Grid.Render();
	// Rendering
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	glDeleteTextures(1, &textureID);

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	//if (VBO!=0)
	/*{
		glDeleteBuffers(1, &VBO);
	}*/
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(App->window->window);
	SDL_Quit();

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//todo: USE MATHGEOLIB here BEFORE 1st delivery! (TIP: Use MathGeoLib/Geometry/Frustum.h, view and projection matrices are managed internally.)
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(ProjectionMatrix.M);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::DrawCubeDirectMode(float originX, float originY, float originZ,float size)
{

	//Vertex
	GLfloat vertices[] =
	{
	 originX + 0 * size ,originY + 0 * size ,originZ + 0 * size, //v0
	 originX + 0 * size ,originY + 1 * size ,originZ + 0 * size, //v1
	 originX + 1 * size ,originY + 1 * size ,originZ + 0 * size, //v2
	 originX + 1 * size ,originY + 0 * size ,originZ + 0 * size, //v3
	 originX + 1 * size ,originY + 0 * size ,originZ + 1 * size, //v4
	 originX + 0 * size ,originY + 0 * size ,originZ + 1 * size, //v5
	 originX + 0 * size ,originY + 1 * size ,originZ + 1 * size, //v6
	 originX + 1 * size ,originY + 1 * size ,originZ + 1 * size, //v7

	};
	//Indices to draw the cube 
	GLubyte indices[] =
	{
	7,6,5,	5,4,7,  //front face 
	2,7,3,	7,4,3,	//rigth face
	1,6,7,	2,1,7,	//top face

	1,0,5,	5,6,1,	//left face
	0,3,5,	5,3,4,	//bottom face
	0,1,2,	2,3,0,	//back face
	};

	// activate and specify pointer to vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	// draw a cube
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);

}

void ModuleRenderer3D::DrawMesh(Mesh* mesh)
{
	glBindTexture(GL_TEXTURE_2D, textureID);

	// activate and specify pointer to vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// activate and specify pointer to vertex array
	glVertexPointer(3, GL_FLOAT, 0, mesh->vertex);

	//Bind normals buffers
	glNormalPointer(GL_FLOAT, 0, mesh->normals);

	glTexCoordPointer(2, GL_FLOAT, 0, mesh->UVs);

	//Draw Lines or Full
	if (wireframeMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Draw mesh
	//glDrawElements(GL_TRIANGLES, mesh->num_index, GL_UNSIGNED_INT, mesh->index);
	glDrawElements(GL_TRIANGLES, mesh->num_index, GL_UNSIGNED_INT, mesh->index);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void ModuleRenderer3D::DrawAllMeshes()
{
	//aasimp::
	auto aux = aasimp::vecMeshes;
	for (int i = 0; i < aux.size(); i++)
	{
		//LOG("DAM activated %d", i)
		DrawMesh(aux.at(i));
	}
}

void ModuleRenderer3D::Checkers()
{
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//void ModuleRenderer3D::LoadImage(const char* filepath)
//{
//	ILuint Name;
//	ilGenImages(1, &Name);
//	ilBindImage(Name);
//
//	if (!ilLoadImage(filepath))
//	{
//		LOG("Image load failed");
//	}
//
//	ilDeleteImages(1, &Name);
//}

