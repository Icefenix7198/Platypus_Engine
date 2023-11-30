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

update_status ModuleScene::PreUpdate(float dt)
{
	if (!pendingToReparent.empty())
	{
		for(int i = 0; i< pendingToReparent.size(); i++)
		{
			GameObject* actual = pendingToReparent.at(i).toMove;
			GameObject* newParent = pendingToReparent.at(i).newParent;
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
			}
		}

		pendingToReparent.empty();
	}

	return UPDATE_CONTINUE;
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

void ModuleScene::RequestReparentGameObject(GameObject* actual, GameObject* newParent)
{
	pendingToReparent.push_back({ actual,newParent });

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




