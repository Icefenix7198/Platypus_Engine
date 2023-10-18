#pragma once
#ifndef MODULE_SCENE
#define MODULE_SCENE

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"


#include <vector>
#include <string>


class ModuleScene : public Module
{
public:

	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init() override;
	bool CleanUp() override;

	////GameObjects related
	GameObject* CreateGameObject(GameObject* parent); //Crea un gameObject vacio
	int UpdateGameObjects(GameObject* go);

	GameObject* root;
private:
	
};

#endif //MODULE_SCENE