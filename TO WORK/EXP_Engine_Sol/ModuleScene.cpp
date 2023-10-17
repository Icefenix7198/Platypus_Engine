#include "Application.h"
#include "ModuleScene.h"
#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"
#include "Globals.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "aasimp.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleScene::~ModuleScene()
{
}

//extern std::string stringTest; //NO PUEDE SER ENCONTRADO

bool ModuleScene::Init()
{  
	return true;
}

bool ModuleScene::CleanUp()
{
	return true;
}




