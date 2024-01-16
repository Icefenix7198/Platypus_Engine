#include "Application.h"
#include "ModuleScene.h"
#include "Globals.h"
#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"

#include <gl/GL.h>
#include <gl/GLU.h>

//JSON for Serialization
#include "parson-master/parson.h"

#include "Component.h"
#include "ComponentTransform.h" //Para updatear la transform cuando el reparent, no se si haya que hacerlo de otra forma y no tener este include
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ComponentParticleSystem.h"

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
				//Add element to its new father list
				newParent->children.push_back(actual);

				//Erase element from old parent list
				actualList->erase(std::find(actualList->begin(), actualList->end(), actual));

				actual->parent = newParent; //Quiza hacerlo asi pueda causar problemas por interrumpir un proceso, por eso esta el disable y enable, pero quiza no sea lo mas seguro.
			}

			//Update transform
			actual->objTransform->ReparentLocalMatrix(newParent);
			actual->objTransform->RecalculateMatrix(false);
		}

		pendingToReparent.clear();
	}

	if (!pendingToDelete.empty())
	{
		for (int i = 0; i < pendingToDelete.size(); i++)
		{
			//Hacemos que deje de apuntar al GameObject el selected
			if (App->scene->selectedGO == pendingToDelete.at(i)) 
			{
				App->scene->selectedGO = App->scene->root; 
			}
			if (pendingToDelete.at(i)->parent != nullptr)
			{
				auto* actualList = &pendingToDelete.at(i)->parent->children;
				//Erase from its parent children vector
				for (int j = 0; j < actualList->size(); j++)
				{
					if(actualList->at(j) == pendingToDelete.at(i));
					{
						actualList->erase(std::find(actualList->begin(), actualList->end(), pendingToDelete.at(i)));
						break;
					}
				}
			}
			pendingToDelete.at(i)->~GameObject();
			//delete pendingToDelete.at(i);
			pendingToDelete.at(i) = nullptr;
			//Doing other thing will cause an exception as we could be trying to access with inspector to smt that doesn't exist
			App->scene->selectedGO = root;
		}

		pendingToDelete.clear();
	}

	if (!pendingToCreate.empty())
	{
		for (int i = 0; i < pendingToCreate.size(); i++)
		{
			GameObject* ref = CreateGameObject(pendingToCreate.at(i).parent, pendingToCreate.at(i).name, pendingToCreate.at(i).UUID);
			//Asumiremos de momento que siempre que queramos crear un GO de momento 
			if(pendingToCreate.at(i).parent == nullptr)
			{
				if (root == nullptr)
				{
					root = ref;
				}
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		GameObject* firework = CreateGameObject(App->scene->root);
		ComponentParticleSystem* pfirework = (ComponentParticleSystem*)firework->CreateComponent(ComponentType::PARTICLE);
		ParticleEmitter* efirework = (ParticleEmitter*)pfirework->CreateEmitter();
		EmitterBase* bfirework = (EmitterBase*)efirework->CreateEmitterByType(BASE);
		bfirework->particlesLifeTime = 5.0;
		EmitterSpawner* sfirework = (EmitterSpawner*)efirework->CreateEmitterByType(SPAWN);
		sfirework->numParticlesToSpawn = 101;
		EmitterPosition* pofirework = (EmitterPosition*)efirework->CreateEmitterByType(POSITION);
		pofirework->direction1 = float3(-0.4, 6.0, 0);
		pofirework->acceleration = true;
		pofirework->particleSpeed1 = 18.00;
		pofirework->particleSpeed2 = 12.200;
		efirework->CreateEmitterByType(ROTATION);
		EmitterSize* sifirework = (EmitterSize*)efirework->CreateEmitterByType(SIZEPARTICLE);
		sifirework->progresive = true;
		sifirework->sizeMultiplier1 = 0.1;
		sifirework->sizeMultiplier2 = 0.411;
		EmitterColor* cofirework = (EmitterColor*)efirework->CreateEmitterByType(COLOR);
		cofirework->progresive = true;
		cofirework->color1 = Color(255, 255, 0, 255);
		cofirework->color2 = Color(255, 255, 255, 0);
		cofirework->startChange = 0.0;
		cofirework->stopChange = 0.2;


		GameObject* explosion = CreateGameObject(App->scene->root);
		ComponentParticleSystem* pexplosion = (ComponentParticleSystem*)explosion->CreateComponent(ComponentType::PARTICLE);
		ParticleEmitter* eexplosion= (ParticleEmitter*)pexplosion->CreateEmitter();
		EmitterBase* bexplosion = (EmitterBase*)eexplosion->CreateEmitterByType(BASE);
		bexplosion->emitterOrigin = float3(0.0,17.1,0.0);
		bexplosion->particlesLifeTime = 5.0;
		EmitterSpawner* sexplosion = (EmitterSpawner*)eexplosion->CreateEmitterByType(SPAWN);
		sexplosion->numParticlesToSpawn = 63;
		EmitterPosition* poexplosion = (EmitterPosition*)eexplosion->CreateEmitterByType(POSITION);
		poexplosion->randomized = true;
		poexplosion->direction1 = float3(-1.0, -1.0, -1.0);
		poexplosion->direction2 = float3(1.0, 1.0, 1.0);
		poexplosion->acceleration = true;
		poexplosion->particleSpeed1 = 2.2;
		poexplosion->particleSpeed2 = 1.1;
		eexplosion->CreateEmitterByType(ROTATION);
		EmitterSize* siexplosion = (EmitterSize*)eexplosion->CreateEmitterByType(SIZEPARTICLE);
		siexplosion->progresive = true;
		siexplosion->sizeMultiplier1 = 0.315;
		siexplosion->sizeMultiplier2 = 0.1;
		EmitterColor* coexplosion = (EmitterColor*)eexplosion->CreateEmitterByType(COLOR);
		coexplosion->progresive = true;
		coexplosion->color1 = Color(255, 0, 0, 0);
		coexplosion->color2 = Color(0, 0, 255, 255);
		coexplosion->startChange = 0.3;
		coexplosion->stopChange = 0.35;
	}

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	return true;
}

GameObject* ModuleScene::CreateGameObject(GameObject* parent,std::string name,uint32_t UUID)
{
	GameObject* go = new GameObject(name,parent,true);
	if (parent != nullptr) { parent->children.push_back(go); }
	go->CreateComponent(ComponentType::TRANSFORM);
	selectedGO = go;
	if (UUID!=0U)
	{
		go->SetUUID(UUID);
	}

	return go;
}

//For the future?
void ModuleScene::RequestCreateGameObject(GameObject* parent, std::string name, uint32_t UUID)
{
	pendingToCreate.push_back({ parent,name,UUID });
}


void ModuleScene::RequestDeleteGameObject(GameObject* go)
{
	pendingToDelete.push_back(go);
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

void ModuleScene::CreateSerializationGameObject(GameObject* go)
{
	JSON_Value* root_value = json_value_init_object();
	JSON_Object* root_object = json_value_get_object(root_value);

	//If there is no failure loading
	if (root_value != nullptr && root_object != nullptr)
	{
		char* serialized_string = NULL;
		//Crear path
		json_object_set_string(root_object, "Name", go->name.c_str());
		json_object_set_number(root_object, "UUID", go->GetUUID());
		json_object_set_boolean(root_object, "Active", go->active);
		json_object_set_boolean(root_object, "IsRoot", go==App->scene->root);
		json_object_set_number(root_object, "NumChildren", go->children.size());
		json_object_set_number(root_object, "NumCompoments", go->components.size());

		//Create array of all Components
		JSON_Array* arr;
		JSON_Value* new_val = json_value_init_array();
		arr = json_value_get_array(new_val);
		json_object_dotset_value(root_object, "Components", new_val);

		//We create the value to assign and insert into the array
		JSON_Value* arrayComponents;
		arrayComponents = json_value_init_object();
		for (size_t i = 0; i < go->components.size(); i++)
		{
			//Components (son object)
			JSON_Value* component;
			component = json_value_init_object();
			JSON_Object* child_object = json_value_get_object(component);

			json_object_set_number(child_object, "UUID", go->components.at(i)->UUID);
			json_object_set_number(child_object, "Type", (int)go->components.at(i)->type);

			//Y aqui un switch para crear en funcion del tipo
			switch (go->components.at(i)->type)
			{
			case TRANSFORM:
			{

				ComponentTransform* cTransform = (ComponentTransform*)go->components.at(i);

				JSON_Array* arrTransform;
				
				//Position
				JSON_Value* jValuePos = json_value_init_array();
				arrTransform = json_value_get_array(jValuePos);

				json_object_dotset_value(child_object, "Position", jValuePos);
				json_array_append_number(arrTransform, cTransform->pos.x);
				json_array_append_number(arrTransform, cTransform->pos.y);
				json_array_append_number(arrTransform, cTransform->pos.z);

				//Rotation
				JSON_Value* jValueRot = json_value_init_array();
				arrTransform = json_value_get_array(jValueRot);

				json_object_dotset_value(child_object, "Rotation", jValueRot);
				json_array_append_number(arrTransform, cTransform->rot.x);
				json_array_append_number(arrTransform, cTransform->rot.y);
				json_array_append_number(arrTransform, cTransform->rot.z);
				json_array_append_number(arrTransform, cTransform->rot.w);

				//Scale
				JSON_Value* jValueScale = json_value_init_array();
				arrTransform = json_value_get_array(jValueScale);

				json_object_dotset_value(child_object, "Scale", jValueScale);
				json_array_append_number(arrTransform, cTransform->scale.x);
				json_array_append_number(arrTransform, cTransform->scale.y);
				json_array_append_number(arrTransform, cTransform->scale.z);
				
				break;
			}
			case MESH:
			{
				ComponentMesh* cMesh = (ComponentMesh*)go->components.at(i);
				Resource* recurso = (Resource*)cMesh->resourceMesh;
				json_object_set_number(child_object, "MeshUUID", recurso->UUID);
				break;
			}
			case MATERIAL:
			{
				break;
			}	
			case CAMERA:
			{
				break;
			}
			case PARTICLE:
			{
				ComponentParticleSystem* cParticle = (ComponentParticleSystem*)go->components.at(i);

				json_object_set_string(child_object,"FilePath", cParticle->SaveMetaEmitters());
				break;
			}
			default:
				break;
			}
			
			json_array_append_value(arr, component);
		}

		JSON_Array* arr2;
		JSON_Value* new_val2 = json_value_init_array();
		arr2 = json_value_get_array(new_val2);
		json_object_dotset_value(root_object, "Children", new_val2);

		//We create the value to assign and insert into the array
		
		for (size_t i = 0; i < go->children.size(); i++)
		{			
			//Childrens
			JSON_Value* children;
			children = json_value_init_object();
			JSON_Object* child_object = json_value_get_object(children);

			json_object_set_string(child_object, "Name", go->children.at(i)->name.c_str());
			json_object_set_number(child_object,"UUID", go->children.at(i)->GetUUID());
			json_array_append_value(arr2, children);

			//Recursivity to create all game objects
			CreateSerializationGameObject(go->children.at(i));
		}

		//Dot set hace que si lo pones en un punto te lo ponga dentro de un {} del punto antes del 
		//Jason_parse_string lo mete en un array
		serialized_string = json_serialize_to_string_pretty(root_value);
		puts(serialized_string);

		//Crear el archivo en assets
		std::string nameMeta;
		nameMeta += ASSETS_GAMEOBJECTS;
		nameMeta += go->name;
		nameMeta += ".json";
		json_serialize_to_file(root_value, nameMeta.c_str());
		json_free_serialized_string(serialized_string);
		json_value_free(root_value);
	}
}

void ModuleScene::InitCreateGOFromSerialization(bool fistTime)
{
	std::vector<std::string> listJsons;
	
	//Get all files with the extension
	App->fileSystem->GetAllFilesWithExtension(ASSETS_GAMEOBJECTS, "json", listJsons);
	
	//Search root node
	for (int i = 0; i < listJsons.size(); ++i)
	{
		//Get add its origin path for the parser to work ("/Assest/Folder/Nombre.json")
		std::string pathFile;
		pathFile.assign(ASSETS_GAMEOBJECTS);
		pathFile += listJsons.at(i).c_str();

		JSON_Value* root_value = json_parse_file(pathFile.c_str());
		JSON_Object* root_object = json_value_get_object(root_value);

		bool isRoot = json_object_get_boolean(root_object, "IsRoot");
		if(isRoot && root_object != NULL)
		{
			//We destroy all old game objects (the childrens of the root)
			for (int j = root->children.size()-1;j>=0;--j) //We do it in the inverse order just in case to not mess with the size of the list
			{
				RequestDeleteGameObject(root->children.at(j));
			}

			//We assign all the values of the new root to the old root (becose we never erase it)
			//For each component on the Json root has we assign the value to the new root
			int numComponents = json_object_get_number(root_object, "NumCompoments");
			JSON_Array* componentsArr = json_object_get_array(root_object, "Components");
			for (int co = 0; co < numComponents; co++)
			{
				//Get the Json object of the array
				JSON_Object* obj = json_array_get_object(componentsArr, co);

				//Get the Type and UUID of the child
				ComponentType type = (ComponentType)json_object_get_number(obj, "Name");
				uint32_t UUID = json_object_get_number(obj, "UUID");

				if (type == ComponentType::TRANSFORM)
				{
					//Assign UUID
					root->objTransform->SetUUID(UUID);

					//Get position array
					JSON_Array* posArr = json_object_get_array(obj, "Position");
					
					//Get elements of position
					float posX = json_array_get_number(posArr, 0);
					float posY = json_array_get_number(posArr, 1);
					float posZ = json_array_get_number(posArr, 2);
					

					//Get rotation array
					JSON_Array* rotArr = json_object_get_array(obj, "Rotation");

					//Get elements of position
					float rotX = json_array_get_number(rotArr, 0);
					float rotY = json_array_get_number(rotArr, 1);
					float rotZ = json_array_get_number(rotArr, 2);
					float rotW = json_array_get_number(rotArr, 3);

					//Get position array
					JSON_Array* scaleArr = json_object_get_array(obj, "Scale");

					//Get elements of position
					float scaleX = json_array_get_number(scaleArr, 0);
					float scaleY = json_array_get_number(scaleArr, 1);
					float scaleZ = json_array_get_number(scaleArr, 2);

					root->objTransform->SetValues({ posX ,posY ,posZ }, { scaleX ,scaleY ,scaleZ }, { rotX ,rotY ,rotZ ,rotW });
				}
			}

			//For each child the Json root has we call the recursive GO creator metod
			int numChilds = json_object_get_number(root_object, "NumChildren");
			JSON_Array* childArr = json_object_get_array(root_object, "Children");
			
			//Navigate array of children 
			for (int c = 0; c < numChilds; ++c)
			{
				//Get the Json object of the array
				JSON_Object* obj = json_array_get_object(childArr, c);
				//Get the Name and UUID of the child
				std::string name = json_object_get_string(obj, "Name");
				uint32_t UUID = json_object_get_number(obj, "UUID");
				
				//Search inside the list of Jsons
				for (int a = 0; a < listJsons.size(); ++a)
				{

					if (strcmp(name.c_str(), App->fileSystem->GetNameFromPath(listJsons.at(a).c_str()).c_str())==0)
					{
						//Test if UUID is correct
						std::string pathJson;
						pathJson.assign(ASSETS_GAMEOBJECTS);
						pathJson += listJsons.at(a).c_str();

						JSON_Value* root_value2 = json_parse_file(pathJson.c_str());
						JSON_Object* root_object2 = json_value_get_object(root_value2);

						//Check if ID is also correct, in case of posible name duplication
						uint32_t ID = json_object_get_number(root_object2, "UUID");
						if(UUID == ID)
						{
							CreateGObFromSerializationRecursively(listJsons, root, App->fileSystem->GetNameFromPath(listJsons.at(a).c_str()).c_str(),ID);
							break;
						}
					}
				}
			}

			break;
		}
	}
}

void ModuleScene::CreateGObFromSerializationRecursively(std::vector<std::string> listJsons, GameObject* parent, const char* jsonName,uint32_t UUID)
{
	//Create this game Object
	//RequestCreateGameObject(parent, jsonName,UUID);
	GameObject* go = CreateGameObject(parent, jsonName, UUID);

	//Create next game objects recursively
	std::string pathFile;
	pathFile.assign(ASSETS_GAMEOBJECTS);
	pathFile += jsonName;
	pathFile += ".json";

	JSON_Value* root_value = json_parse_file(pathFile.c_str());
	JSON_Object* root_object = json_value_get_object(root_value);

	//For each component on the Json root has we craete a new 
	int numComponents = json_object_get_number(root_object, "NumCompoments");
	JSON_Array* componentsArr = json_object_get_array(root_object, "Components");
	for (int co = 0; co < numComponents; co++)
	{
		//Get the Json object of the array
		JSON_Object* obj = json_array_get_object(componentsArr, co);

		//Get the Type and UUID of the child
		ComponentType type = (ComponentType)json_object_get_number(obj, "Type");
		uint32_t UUID = json_object_get_number(obj, "UUID");

		switch (type)
		{
		case TRANSFORM:
		{
			//Get position array
			JSON_Array* posArr = json_object_get_array(obj, "Position");

			go->objTransform->SetUUID(UUID);

			//Get elements of position
			float posX = json_array_get_number(posArr, 0);
			float posY = json_array_get_number(posArr, 1);
			float posZ = json_array_get_number(posArr, 2);


			//Get rotation array
			JSON_Array* rotArr = json_object_get_array(obj, "Rotation");

			//Get elements of position
			float rotX = json_array_get_number(rotArr, 0);
			float rotY = json_array_get_number(rotArr, 1);
			float rotZ = json_array_get_number(rotArr, 2);
			float rotW = json_array_get_number(rotArr, 3);

			//Get position array
			JSON_Array* scaleArr = json_object_get_array(obj, "Scale");

			//Get elements of position
			float scaleX = json_array_get_number(scaleArr, 0);
			float scaleY = json_array_get_number(scaleArr, 1);
			float scaleZ = json_array_get_number(scaleArr, 2);

			go->objTransform->SetValues({ posX ,posY ,posZ }, { scaleX ,scaleY ,scaleZ }, { rotX ,rotY ,rotZ ,rotW });
			break;
		}
		case MESH:
		{
			ComponentMesh* coMesh = (ComponentMesh*)go->CreateComponent(type);
			coMesh->SetUUID(UUID);
			uint32_t MeshID = json_object_get_number(obj, "MeshUUID"); //En principio no hay que acceder mas profundo porque esta en el object que es cada componente
			Resource* resMesh = new ResourceMesh;
			resMesh->type = ResourceType::MESH;
			resMesh->UUID = MeshID;
			std::string pathIDLibrary;
			pathIDLibrary = MESHES_PATH;
			pathIDLibrary += std::to_string(MeshID); //JSON UUID AQUI
			pathIDLibrary += CFF;
			coMesh->resourceMesh->LoadFromLibrary(pathIDLibrary.c_str(), resMesh);
			coMesh->resourceMesh = (ResourceMesh*)resMesh;
			//coMesh->IDResourceMesh;
			break;
		}
		case MATERIAL:
		{
			ComponentMaterial* coMaterial = (ComponentMaterial*)go->CreateComponent(type);
			break;
		}
		case CAMERA:
		{
			//ComponentCamera* coMesh = (ComponentCamera*)go->CreateComponent(type); //No se puede hacer porque ANDREU ni siquiera lo hizo un componente
			break;
		}
		case PARTICLE:
		{
			ComponentParticleSystem* coParticle = (ComponentParticleSystem*)go->CreateComponent(type);

			uint32_t ID = json_object_get_number(obj, "UUID");
			coParticle->SetUUID(UUID);
			std::string pathJSON;

			pathJSON += ASSETS_PARTICLES;
			pathJSON += std::to_string(ID);
			pathJSON += ".meta";

			coParticle->LoadAllEmmitersJSON(pathJSON.c_str());
			break;
		}
		default:
			break;
		}
	}
	int numChilds = json_object_get_number(root_object, "NumChildren");
	JSON_Array* childArr = json_object_get_array(root_object, "Children");

	//Navigate array of children 
	for (int i = 0; i < numChilds; ++i)
	{
		//Get the Json object of the array
		JSON_Object* obj = json_array_get_object(childArr, i);
		//Get the Name and UUID of the child
		std::string name = json_object_get_string(obj, "Name");
		uint32_t UUID = json_object_get_number(obj, "UUID");

		//Search inside the list of Jsons
		for (int j = 0; j < listJsons.size(); ++j)
		{

			if (strcmp(name.c_str(), App->fileSystem->GetNameFromPath(listJsons.at(j).c_str()).c_str()) == 0)
			{
				//Test if UUID is correct
				std::string pathJson;
				pathJson.assign(ASSETS_GAMEOBJECTS);
				pathJson += listJsons.at(j).c_str();

				JSON_Value* root_value2 = json_parse_file(pathJson.c_str());
				JSON_Object* root_object2 = json_value_get_object(root_value2);

				//Check if ID is also correct, in case of posible name duplication
				uint32_t ID = json_object_get_number(root_object2, "UUID");
				if (UUID == ID)
				{
					CreateGObFromSerializationRecursively(listJsons, go, App->fileSystem->GetNameFromPath(listJsons.at(j).c_str()).c_str(),ID);
					break;
				}
			}
		}
	}
}


