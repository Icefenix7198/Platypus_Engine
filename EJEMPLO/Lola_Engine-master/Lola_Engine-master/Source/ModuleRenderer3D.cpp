#include "ModuleRenderer3D.h"

#include "Application.h"
#include "ModuleCameraEditor.h"
#include "ModuleWindow.h"
#include "Globals.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ComponentCamera.h"
#include "GameObject.h"

#include "ResourceMesh.h"
#include "Mesh.h"

#include "openGL.h"

#include "Devil\include\ilut.h"

#pragma comment (lib, "glu32.lib")						 /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib")					 /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Source/Glew/libx86/glew32.lib")   /* link Glew lib   */

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "Renderer3D");
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Awake(JSONParser &config)
{
	DEBUG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		DEBUG("[error] OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	sprintf_s(gl_version, SHORT_STRING, "%s", glGetString(GL_VERSION));

	GLenum gl_enum = glewInit();

	if (gl_enum != GLEW_OK)
		DEBUG("[error] Glew hasn't been initialized!");

	sprintf_s(glew_version, SHORT_STRING, "%s", glewGetString(GLEW_VERSION));
	
	if(ret == true)
	{
		//Use Vsync
		if(config.GetBoolean("V-Sync") && SDL_GL_SetSwapInterval(1) < 0)
			DEBUG("[error] Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			DEBUG("[error] Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			DEBUG("[error] Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//clear_color = ImColor(114, 144, 154);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			DEBUG("[error] Error initializing OpenGL! %s\n", gluErrorString(error));
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

	// Projection matrix and view matrix
	view_matrix.SetIdentity();
	projection_matrix.SetIdentity();
	projection_matrix_pending = true;

	// Default texture
	GenerateChecker(&checker_id);

	return ret;
}

// PreUpdate: clear buffer
UPDATE_STATUS ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (projection_matrix_pending)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		CalculateProjectionMatrix();
		glLoadMatrixf(*projection_matrix.v);
		
		projection_matrix_pending = false;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	CalculateViewMatrix();
	glLoadMatrixf(*view_matrix.v);

	// light 0 on cam pos
	math::vec camera_pos = App->camera->GetCameraPos();
	lights[0].SetPos(camera_pos.x, camera_pos.y, camera_pos.z);
	//lights[1].SetPos(100.0f, 100.0f, 100.0f);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// Update present buffer to screen
UPDATE_STATUS ModuleRenderer3D::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleRenderer3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleRenderer3D::SwapBuffers()
{
	// Rendering
	glViewport(0, 0, App->window->GetScreenWidth(), App->window->GetScreenHeight());
	SDL_GL_SwapWindow(App->window->window);
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	DEBUG("Destroying 3D Renderer");
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::SetFrustumForProjection(const math::Frustum &frustum)
{
	this->proj_frustum = frustum;
	projection_matrix_pending = true;
}

void ModuleRenderer3D::SetFrustumForView(const math::Frustum &frustum)
{
	this->view_frustum = frustum;	
}

void ModuleRenderer3D::CalculateProjectionMatrix()
{
	projection_matrix = proj_frustum.ComputeProjectionMatrix();
	projection_matrix = projection_matrix.Transposed();
}

void ModuleRenderer3D::CalculateViewMatrix()
{
	math::float4x4 view = view_frustum.ComputeViewMatrix();
	view_matrix = view.Transposed();
}

void ModuleRenderer3D::GenerateChecker(uint *buffer)
{
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0)^(((j & 0x8)) == 0))) * 255;
			check_image[i][j][0] = (GLubyte)c;
			check_image[i][j][1] = (GLubyte)c;
			check_image[i][j][2] = (GLubyte)c;
			check_image[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, buffer);
	glBindTexture(GL_TEXTURE_2D, *buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
	0, GL_RGBA, GL_UNSIGNED_BYTE, check_image);
}

void ModuleRenderer3D::DrawDirectMode()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	
	// --- Array Vertexs --- drawn by index: see Index Array
	const int num_vertices = 8;
	math::vec *vertices = new math::vec[num_vertices];
	int i = 0;

	/*       6-------7
	       / ¦     / |
		  2 -¦--- 3  |   Y     X
		  |  4----|- 5   |   /                 
          | /     | /    | /
		  0 ----- 1	     O __ __ Z
	*/

	vertices[i++] = { 0.0f, 0.0f, 0.0f };  // 0
	vertices[i++] = { 0.0f, 0.0f, 2.0f };  // 1
	vertices[i++] = { 0.0f, 2.0f, 0.0f };  // 2
	vertices[i++] = { 0.0f, 2.0f, 2.0f };  // 3
	vertices[i++] = { 2.0f, 0.0f, 0.0f };  // 4
	vertices[i++] = { 2.0f, 0.0f, 2.0f };  // 5
	vertices[i++] = { 2.0f, 2.0f, 0.0f };  // 6
	vertices[i++] = { 2.0f, 2.0f, 2.0f };  // 7

	uint my_id = 0;	
	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	//glDrawArrays(GL_TRIANGLES, 0, num_vertices * 3);

	// --- Index Array ----
	const int num_indices = 36;
	uint *indices = new uint[num_indices];
	i = 0;

	// Front
	indices[i++] = 0;
	indices[i++] = 1;
	indices[i++] = 2;
	indices[i++] = 1;
	indices[i++] = 3;
	indices[i++] = 2;

	// Right
	indices[i++] = 3;
	indices[i++] = 1;
	indices[i++] = 5;
	indices[i++] = 3;
	indices[i++] = 5;
	indices[i++] = 7;

	// Left
	indices[i++] = 4;
	indices[i++] = 0;
	indices[i++] = 2;
	indices[i++] = 2;
	indices[i++] = 6;
	indices[i++] = 4;

	// Rear
	indices[i++] = 6;
	indices[i++] = 5;
	indices[i++] = 4;
	indices[i++] = 6;
	indices[i++] = 7;
	indices[i++] = 5;

	// Top
	indices[i++] = 6;
	indices[i++] = 2;
	indices[i++] = 3;
	indices[i++] = 6;
	indices[i++] = 3;
	indices[i++] = 7;

	// Bottom
	indices[i++] = 0;
	indices[i++] = 4;
	indices[i++] = 1;
	indices[i++] = 4;
	indices[i++] = 5;
	indices[i++] = 1;

	uint my_indices = 0;
	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_indices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleRenderer3D::ShowGameObject(const GameObject *go)
{
	const ComponentMesh *comp_mesh = nullptr;
	const ComponentTexture *comp_tex = nullptr;

	comp_mesh = (ComponentMesh*)go->GetComponentByType(COMPONENT_TYPE::COMP_MESH);
	comp_tex = (ComponentTexture*)go->GetComponentByType(COMPONENT_TYPE::COMP_MATERIAL);

	// Rendering
	if (comp_mesh != nullptr && comp_mesh->IsActive())
	{
		// Transformation 
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMultMatrixf(*go->transform->world_transform.Transposed().v);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, comp_mesh->resource->mesh_data->id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, comp_mesh->resource->mesh_data->id_normals);
		glNormalPointer(GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, comp_mesh->resource->mesh_data->id_tex_coord);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);		

		if (comp_mesh->wire)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (comp_tex != nullptr && comp_tex->IsActive())
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0); // Cleanning bind buffer;

				//// Alpha testing-> Add this alpha test on the corresponding resource 
				//glEnable(GL_ALPHA_TEST);
				//glAlphaFunc(GL_GREATER, 0.1f);

				////Blending 
				//glEnable(GL_BLEND);
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glBindTexture(GL_TEXTURE_2D, comp_tex->resource->tex_buffer);		
				//glColor3f(comp_mat->resource->color_diffuse.x, comp_mat->resource->color_diffuse.y, comp_mat->resource->color_diffuse.z);
			}
		}			

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, comp_mesh->resource->mesh_data->id_indices);
		glDrawElements(GL_TRIANGLES, comp_mesh->resource->mesh_data->num_indices, GL_UNSIGNED_INT, NULL);
		glColor3f(1.0f, 1.0f, 1.0f);

		 //Is this GO selected?
		if (go->selected)
		{
			glLineWidth(1.5f);
			glColor3f(1.0f, 1.0f, 0.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, comp_mesh->resource->mesh_data->num_indices, GL_UNSIGNED_INT, NULL);
			glColor3f(1.0f, 1.0f, 1.0f);
			glLineWidth(1.0f);
		}

		glPopMatrix();
		
		glDisable(GL_TEXTURE_2D);
		/*glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);*/

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}		
}