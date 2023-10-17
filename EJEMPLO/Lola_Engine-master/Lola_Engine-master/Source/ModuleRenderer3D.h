#ifndef __MODULERENDERER3D_H__
#define __MODULERENDERER3D_H__

#include "Module.h"
#include "Globals.h"
#include "Light.h"

#include "imgui\imgui.h"
#include "MathGeoLib\MathGeoLib.h"
#include "SDL\include\SDL_video.h"

#define MAX_LIGHTS 8
#define CHECKERS_HEIGHT 512
#define CHECKERS_WIDTH 512

struct Mesh;
class GameObject;

class ModuleRenderer3D : public Module
{
public:

	char gl_version[SHORT_STRING], glew_version[SHORT_STRING];

	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Awake(JSONParser &config);
	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS Update(float dt);
	UPDATE_STATUS PostUpdate(float dt);
	bool CleanUp();

	void SetFrustumForProjection(const math::Frustum &frustrum);
	void SetFrustumForView(const math::Frustum &frustrum);
	void CalculateProjectionMatrix();
	void CalculateViewMatrix();

	void ShowGameObject(const GameObject *go);

	void DrawDirectMode();
	void SwapBuffers();

public:

	bool projection_matrix_pending = false;
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	math::float4x4 projection_matrix, view_matrix;

private:

	math::Frustum proj_frustum, view_frustum;

	unsigned int checker_id;
	unsigned char check_image[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	void GenerateChecker(unsigned int*buffer);	
};

#endif // !__MODULERENDERER3D_H__