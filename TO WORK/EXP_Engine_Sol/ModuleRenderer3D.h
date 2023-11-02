#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"

#include "MathGeoLib/include/Math/float3x3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "Primitive.h"
#include "Glew/include/glew.h"
#include "aasimp.h"

//todo: REMOVE this before 1st delivery!!
#include "glmath.h"

#define MAX_LIGHTS 8

#define CHECKERS_HEIGHT 256
#define CHECKERS_WIDTH 256

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void DrawCubeDirectMode(float originX=0, float originY=0, float originZ=0, float size=1);
	
	void DrawMesh(Mesh* mesh, bool wireframe = false);
	void DrawAllMeshes();
	void Checkers();

	//Drawing Mode
	bool GetDrawingMode() { return wireframeMode; }
	bool GetDepthTest() { return glDepthTest; }
	bool GetCullFace() { return glCullFace; }
	bool GetLigthning() { return glLigthning; }
	bool GetColorMaterial() { return glColorMaterial; }
	bool GetTexture2D() { return glTexture2D; }
	//bool GetDrawingMode() { return wireframeMode; }

	void SetDrawingMode(bool mode) { wireframeMode = mode; }
	void SetDepthTest(bool mode) { glDepthTest = mode; }
	void SetCullFace(bool mode) { glCullFace = mode; }
	void SetLigthning(bool mode) { glLigthning = mode; }
	void SetColorMaterial(bool mode) { glColorMaterial = mode; }
	void SetTexture2D(bool mode) { glTexture2D = mode; }

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	CPlane Grid;

	bool second_Window = true; 
	
	//You won't need this after using Frustum
	mat4x4 ProjectionMatrix;

	GLuint VBO; //VERTEX BUFFER OBJECT
	GLuint EBO; //ELEMENT BUFFER OBJECT
	GLuint VAO; //VERTEX ATRIBUTE OBJECT

	GLuint textureID;
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

private:
	//Drawing Modes
	bool wireframeMode = false;

	//FLAGS OPENGL
	bool glDepthTest = true;
	bool glCullFace = true;
	bool glLigthning = true;
	bool glColorMaterial = true;
	bool glTexture2D = true;
};