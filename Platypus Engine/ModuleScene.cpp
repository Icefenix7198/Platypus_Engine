#include "Application.h"
#include "ModuleScene.h"
#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"
#include "Globals.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "aasimp.h"

#include "Component.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//std::string name;
	//name.assign("Root_Node");

	//root = new GameObject(name, nullptr); //Create the root node
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{  
	std::string name;
	name.assign("Root_Node");

	root = CreateGameObject(nullptr, name);

	return true;
}

bool ModuleScene::CleanUp()
{
	return true;
}

GameObject* ModuleScene::CreateGameObject(GameObject* parent,std::string name)
{
	GameObject* go = new GameObject(name,parent,true);
	if (parent != nullptr) { parent->children.push_back(go); }
	go->CreateComponent(ComponentType::TRANSFORM);
	selectedGO = go;

	return go;
}

GameObject* ModuleScene::ReparentGameObject(GameObject* actual, GameObject* newParent)
{
	GameObject* aux = nullptr;
	GameObject* ret = nullptr;
	bool last_state = actual->active;
	auto* actualList = &actual->parent->children;
	if (actual != nullptr && newParent != nullptr)
	{
		//Precaution Measure
		actual->active = false;
		actual->parent->active = false;

		//Add element to its new father list
		newParent->children.push_back(actual);

		//Erase element from old parent list
		actualList->erase(std::find(actualList->begin(), actualList->end(), actual));
		
		actual->parent = newParent; //Quiza hacerlo asi pueda causar problemas por interrumpir un proceso, por eso esta el disable y enable, pero quiza no sea lo mas seguro.
		actual->active = last_state;
	}
	

	return ret;
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




