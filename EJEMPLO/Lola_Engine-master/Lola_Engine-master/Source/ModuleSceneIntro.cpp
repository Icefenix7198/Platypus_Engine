#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "Scene");
}

ModuleSceneIntro::~ModuleSceneIntro()
{ }

// Load assets
bool ModuleSceneIntro::Start()
{
	DEBUG("Loading Intro scene");
	bool ret = true;

	return ret;
}

// Cleaning assets
bool ModuleSceneIntro::CleanUp()
{
	DEBUG("Unloading Intro scene");
	return true;
}

// Update
UPDATE_STATUS ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return UPDATE_CONTINUE;
}

