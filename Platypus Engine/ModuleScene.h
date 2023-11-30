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
	GameObject* CreateGameObject(GameObject* parent,std::string name = "GameObject"); //Crea un gameObject vacio
	GameObject* ReparentGameObject(GameObject* actual,GameObject* newParent); //Hace el reparent, retorn nullptr si fallo, sino el nuevo padre del objeto.
	int UpdateGameObjects(GameObject* go);

	GameObject* root;
	GameObject* selectedGO = nullptr;
private:
	
};

#endif //MODULE_SCENE