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
	root = new GameObject("Root_Node", nullptr); //Create the root node
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{  
	return true;
}

bool ModuleScene::CleanUp()
{
	return true;
}

GameObject* ModuleScene::CreateGameObject(GameObject* parent)
{
	GameObject* go = new GameObject;
	
	parent->children.push_back(go);

	return go;
}

int ModuleScene::UpdateGameObjects(GameObject* go)
{
	int numChildren = 0;
	for (int i = 0; i < go->children.size(); i++)
	{
		if (go->children.at(i)->active)
		{
			numChildren++;
			numChildren += UpdateGameObjects(go->children.at(i));

			//Make GameObjects update
			go->children.at(i)->Update();
		}	
	}

	return 0;
}




