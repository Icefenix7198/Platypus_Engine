#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_sdl2.h"
#include "ImGui/backends/imgui_impl_opengl3.h"


#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

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
	}

	// Projection matrix for
	OnResize(App->window->GetScreenWidth(), App->window->GetScreenHeigth());

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
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
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, context);
	ImGui_ImplOpenGL3_Init("#version 130");
	//iNICIAR GL GLEW (ESTA EN EL POWER)

	Grid.axis = true;

	
	
	//VBO = 0; //Buffer de vertices
	//EBO = 0;
	//VAO = 0;

	////Generate buffers.If after this any of them is 0 there is an error
	//glGenBuffers(1, &VBO); 
	//glGenBuffers(1, &EBO);
	//glGenVertexArrays(1, &VAO);

	//std::vector<float> vecVertices;
	//std::vector<GLuint> vecIndices;

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, vecVertices.capacity(), vecVertices.data(), GL_STATIC_DRAW); //Bien declarada pero hay que darle un arrayVertices que exista
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ARRAY_BUFFER, vecIndices.capacity(), vecIndices.data(), GL_STATIC_DRAW); //Bien declarada pero hay que darle un arrayIndices que exista
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//
	////VAO declaration is special/different
	//glBindVertexArray(VAO);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //???,num elements del tipo, tipo VAR, ???, tamaño bite elementos, offset en bites (por si hay elementos anteriores)
	//glEnableVertexAttribArray(0);
	//glBindVertexArray(0);

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

	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

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
	//DrawSphereDirectMode(3, 8, 5); //This is broken and doesen't work
	DrawPyramidDirectMode(0,0,0,7,5,5);

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	App->editor->DrawEditor();
	

	// Rendering
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	//glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
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

	Grid.Render();
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

void ModuleRenderer3D::DrawSphereDirectMode(float radius, unsigned int rings, unsigned int sectors) // No funciona
{
	std::vector<GLushort> indices;
	std::vector<vec3> vertices;
	std::vector<vec2> texcoords;

	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);

	for (int r = 0; r < rings; ++r) {
		for (int s = 0; s < sectors; ++s) {
			float const y = sin(-M_PI_2 + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			texcoords.push_back(vec2(s * S, r * R));
			vertices.push_back(vec3(x, y, z) * radius);

			int curRow = r * sectors;
			int nextRow = (r + 1) * sectors;

			indices.push_back(curRow + s);
			indices.push_back(nextRow + s);
			indices.push_back(nextRow + (s + 1));

			indices.push_back(curRow + s);
			indices.push_back(nextRow + (s + 1));
			indices.push_back(curRow + (s + 1));
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices.data());
	//glTexCoordPointer(2, GL_FLOAT, 0, texcoords.data());

	//glEnable(GL_TEXTURE_2D);
	////glFrontFace(GL_CCW);
	//glEnable(GL_CULL_FACE);
	///*glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, privModel->texname);*/

	glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_SHORT, indices.data());
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	

}

void ModuleRenderer3D::DrawPyramidDirectMode(float originX, float originY, float originZ, uint numFaces,float heigth, float width)
{
	std::vector<float> x;
	std::vector<float> y;
	std::vector<float> z;

	//Top point
	x.push_back(originX);
	y.push_back(originY);
	z.push_back(originZ);

	//Create lateral faces
	if (numFaces >= 3)
	{
		float degree = PI / numFaces;
		float h = originY - heigth;
		//Pushback all vertices
		for (uint i = 0; i < numFaces; i++)
		{
			y.push_back(h); //All pyramid points are in the same heigth

			//Vertices
			x.push_back(originX+width * sin(0 + degree * i));
			z.push_back(originZ + width * cos(0 + degree * i));
		}
	}

	//Add lateral faces indexes
	std::vector<int> indices;

	for (int i = 1; i <= numFaces; i++)
	{
		indices.push_back(0);
		indices.push_back(i);
		if (i>=numFaces) //Si ya hemos dado la vuelta le devolvemos un 1 (el primer vertice lateral
		{
			indices.push_back(1);
		}
		else { indices.push_back(i + 1); }
	}
	
	//Add base indexes
	//Note: Due to the order that they must be drawn to be seen (horario) is best to start from the end

	int extraOffset = 0;
	for (int i = 0; i < numFaces-2 ; i++)
	{
		indices.push_back(numFaces);
		for (int j = 1; j < 3; j++)
		{
			int val = numFaces - 2 * i - j;
			indices.push_back(val);
		}

	}

	std::vector<float> vertices;
	for (size_t i = 0; i < x.size(); i++) //Podria ser x, y o z ya que son exactamente iguales
	{
		vertices.push_back(x.at(i));
		vertices.push_back(y.at(i));
		vertices.push_back(z.at(i));
	}

	// activate and specify pointer to vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices.data());

	// draw a cube
	glDrawElements(GL_TRIANGLES, indices.size()/3, GL_UNSIGNED_BYTE, indices.data());

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);


	//GLfloat vertices[3+numFaces*3];
}
