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
	std::string name;
	name.assign("Root_Node");

	root = new GameObject(name, nullptr); //Create the root node
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{  
	selectedGO = root;

	return true;
}

bool ModuleScene::CleanUp()
{
	return true;
}

GameObject* ModuleScene::CreateGameObject(GameObject* parent,std::string name)
{
	GameObject* go = new GameObject;
	go->name = name;
	parent->children.push_back(go);
	selectedGO = go;

	return go;
}

int ModuleScene::UpdateGameObjects(GameObject* go)
{
	int numChildren = 0;
	for (int i = 0; i < go->children.size(); i++) //Iterate all children gameObjects
	{
		if (go->children.at(i)->active)
		{
			numChildren++;
			numChildren += UpdateGameObjects(go->children.at(i));

			
		}	
	}
	
	//Make GameObject update
	go->Update();

	return numChildren; //Returns the number of active GameObjects
}




