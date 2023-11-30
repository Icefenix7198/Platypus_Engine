#pragma once
#ifndef MODULE_SCENE
#define MODULE_SCENE

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"


#include <vector>
#include <string>

struct reParentPair
{
	GameObject* toMove;
	GameObject* newParent;
};

class ModuleScene : public Module
{
public:

	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init() override;
	update_status PreUpdate(float dt) override; 
	bool CleanUp() override;

	////GameObjects related
	GameObject* CreateGameObject(GameObject* parent,std::string name = "GameObject"); //Crea un gameObject vacio
	void RequestReparentGameObject(GameObject* actual,GameObject* newParent); //Hace el reparent, retorn nullptr si fallo, sino el nuevo padre del objeto.
	int UpdateGameObjects(GameObject* go);

	GameObject* root;
	GameObject* selectedGO = nullptr;

	
private:
	std::vector<reParentPair> pendingToReparent;
};

#endif //MODULE_SCENE