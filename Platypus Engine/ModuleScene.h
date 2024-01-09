#pragma once
#ifndef MODULE_SCENE
#define MODULE_SCENE

#include "Module.h"
//#include "Globals.h"
#include "GameObject.h"


#include <vector>
#include <string>

struct reParentPair
{
	GameObject* toMove;
	GameObject* newParent;
};

struct newGameObject
{
	GameObject* parent;
	std::string name;
	uint32_t UUID;
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
	GameObject* CreateGameObject(GameObject* parent,std::string name = "GameObject",uint32_t UUID = 0); //Crea un gameObject vacio
	void RequestCreateGameObject(GameObject* parent, std::string name = "GameObject", uint32_t UUID = 0);
	void RequestReparentGameObject(GameObject* actual,GameObject* newParent); //Hace el reparent, retorn nullptr si fallo, sino el nuevo padre del objeto.
	void RequestDeleteGameObject(GameObject* go);
	int UpdateGameObjects(GameObject* go);

	GameObject* root;
	GameObject* selectedGO = nullptr;

	//Serialization
	void CreateSerializationGameObject(GameObject* go);
	void InitCreateGOFromSerialization();
	void CreateGObFromSerializationRecursively(std::vector<std::string> listJsons, GameObject* go, const char* jsonName,uint32_t UUID);

private:
	std::vector<reParentPair> pendingToReparent;
	std::vector<GameObject*> pendingToDelete;
	std::vector<newGameObject> pendingToCreate;
};

#endif //MODULE_SCENE