#include "ModuleGameObjectManager.h"

#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCameraEditor.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"

#include "GameObject.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

#include "Mesh.h"

#include <stack>
#include <algorithm> 

ModuleGameObjectManager::ModuleGameObjectManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "GameObject Manager");
}

// Destructor
ModuleGameObjectManager::~ModuleGameObjectManager()
{ }

// Called before render is available
bool ModuleGameObjectManager::Awake(JSONParser &config)
{
	bool ret = true;

	// First gameobject of the scene (UUID = 0)
	CreateRoot();

	// Initializing OcTree	
	oc_tree_boundaries.SetFromCenterAndSize({ 0.0f, 0.0f, 0.0f }, { 100.0f, 100.0f, 100.0f });
	oc_tree.SetBoundaries(oc_tree_boundaries);

	return ret;
}

// PreUpdate: clear buffer
UPDATE_STATUS ModuleGameObjectManager::PreUpdate(float dt)
{
	bool update_needed = false;

	// Looking GameObject to delete...
	std::vector<GameObject*>::iterator it;
	
	for(it = list_of_gos.begin(); it != list_of_gos.end(); ++it)
	{
		if ((*it) != nullptr && (*it)->to_delete)
		{
			DeleteGameObject((*it));
			(*it) = nullptr;
			update_needed = true;
		}
	}

	// Deleting the erased GameObjects form the list of GOs
	for (it = list_of_gos.begin(); it != list_of_gos.end();)
	{
		if ((*it) == nullptr)
			it = list_of_gos.erase(it);
		else
			++it;
	}

	if (update_needed) UpdateOcTree();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleGameObjectManager::Update(float dt)
{
	GameObject *curr_go = nullptr;
		
	// Run Update() for each component!
	for (uint i = 0; i < list_of_gos.size(); ++i)
	{
		curr_go = list_of_gos[i];
		
		if (curr_go->IsActive())
		{
			for (uint i = 0; i < curr_go->components.size(); ++i)
				curr_go->components[i]->Update();				
		}
	}

	// Frustum culling
	const GameObject *camera = App->camera->GetEditorCamera();
	math::Frustum frustum = ((ComponentCamera*)camera->GetComponentByType(COMPONENT_TYPE::COMP_CAMERA))->cam_frustum;
	FrustumCulling(frustum);

	for (uint i = 0; i < list_of_gos_to_draw.size(); ++i)
	{
		const GameObject *curr_go = list_of_gos_to_draw[i];
		if(curr_go->active)
			App->renderer3D->ShowGameObject(curr_go);	
	}	

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleGameObjectManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGameObjectManager::CleanUp()
{
	bool ret = true;

	for (uint i = 0; i < list_of_gos.size(); ++i)
		RELEASE(list_of_gos[i]);

	list_of_gos.clear();
	list_of_gos_to_draw.clear();
	
	return ret;
}

// Draw debug information
void ModuleGameObjectManager::DrawDebug()
{
	const GameObject *curr_go = nullptr;

	for (uint i = 0; i < list_of_gos.size(); ++i)
	{
		curr_go = list_of_gos[i];

		for (uint j = 0; j < curr_go->components.size(); ++j)
		{
			switch (curr_go->components[j]->GetType())
			{
			case(COMPONENT_TYPE::COMP_MESH):
				App->debug_mode.DrawAABB(((ComponentMesh*)curr_go->components[j])->bounding_box);
				break;
			case(COMPONENT_TYPE::COMP_CAMERA):
				App->debug_mode.DrawFrustum(((ComponentCamera*)curr_go->components[j])->cam_frustum);

				if (curr_go == App->camera->GetEditorCamera())
					App->debug_mode.DrawOcTree(oc_tree, ((ComponentCamera*)curr_go->components[j])->cam_frustum);
				break;
			}
		}		
	}	
}

const GameObject *ModuleGameObjectManager::GetRoot() const
{
	return root;
}

GameObject *ModuleGameObjectManager::Get(long unsigned int ID) const
{
	for (uint i = 0; i < list_of_gos.size(); ++i)
	{
		if (list_of_gos[i]->UUID == ID)
			return list_of_gos[i];
	}
	return nullptr;
}

GameObject *ModuleGameObjectManager::CreateGameObject(const char *name)
{
	GameObject *new_go = nullptr;
	new_go = new GameObject(name);
	list_of_gos.push_back(new_go);

	return new_go;
}

GameObject *ModuleGameObjectManager::CreateGameObject(const char *name, GameObject *parent)
{
	GameObject *new_go = CreateGameObject(name);
	
	if (parent != nullptr) // With not nullptr pointer parent, argument parent will be used.
	{
		new_go->parent = parent;
		new_go->parent_UUID = parent->UUID;
		parent->children.push_back(new_go);
	}
	else // With nullptr pointer parent, root will be the parent.
	{
		new_go->parent = root;
		new_go->parent_UUID = 0;
		root->children.push_back(new_go);
	}

	return new_go;
}

void ModuleGameObjectManager::CreateRoot()
{
	root = new GameObject("Root");
	root->UUID = 0;
	list_of_gos.push_back(root);
}

GameObject *ModuleGameObjectManager::GetGameObject(long unsigned int UUID_to_search, const std::vector<GameObject*> &list_to_check) const
{
	GameObject *go = nullptr;

	for (uint i = 0; i < list_to_check.size(); ++i)
	{
		if (list_to_check[i]->UUID == UUID_to_search)
			return list_to_check[i];
	}

	return nullptr;
}

void ModuleGameObjectManager::MarkChildToDelete(GameObject *go)
{
	std::stack<GameObject*> go_stack;
	go_stack.push(go);

	GameObject *curr_go = nullptr;

	while (!go_stack.empty())
	{
		curr_go = go_stack.top();
		go_stack.pop();

		curr_go->to_delete = true;

		for (uint i = 0; i < curr_go->children.size(); ++i)
			go_stack.push(curr_go->children[i]);
	}
}

bool ModuleGameObjectManager::DeleteGameObject(GameObject *go_to_delete)
{
	bool ret = false;

	if (go_to_delete != nullptr)
	{
		// Removing child (go_to_delete) from the children parent's list.
		if (go_to_delete->parent != nullptr)
		{
			for (std::vector<GameObject*>::iterator it = go_to_delete->parent->children.begin(); it != go_to_delete->parent->children.end(); ++it)
			{
				if ((*it) == go_to_delete)
				{
					go_to_delete->parent->children.erase(it);
					break;
				}
			}
		}

		// Removing (go_to_delete) as parent from its children.
		for (uint i = 0; i < go_to_delete->children.size(); ++i)
			go_to_delete->children[i]->parent = nullptr;

		delete go_to_delete; //RELEASE(go_to_delete);
		ret = true;		
	}	

	return ret;
}

bool ModuleGameObjectManager::DeleteGameObject(long unsigned int id_to_delete)
{
	return DeleteGameObject(GetGameObject(id_to_delete, list_of_gos));
}

void ModuleGameObjectManager::SetEditorCamera(const ComponentCamera *comp_cam)
{
	ComponentCamera *cam = nullptr;

	for (uint i = 0; i < list_of_gos.size(); ++i)
	{				
		cam = (ComponentCamera*)list_of_gos[i]->GetComponentByType(COMPONENT_TYPE::COMP_CAMERA);
		
		if (cam && cam != comp_cam)
			cam->editor_camera = false;
	}

	App->camera->SetAsEditorCamera(comp_cam->game_object);
}

void ModuleGameObjectManager::CreateCamera()
{
	GameObject *go = CreateGameObject("Camera", nullptr);
	go->AddComponent(COMPONENT_TYPE::COMP_TRANSFORM);
	go->AddComponent(COMPONENT_TYPE::COMP_CAMERA);
}

int ModuleGameObjectManager::FrustumCulling(const math::Frustum &frustum)
{
	list_of_gos_to_draw.clear();
	oc_tree.CollectCandidates(list_of_gos_to_draw, frustum);
	
	return list_of_gos_to_draw.size();
}

bool ModuleGameObjectManager::Save(JSONParser &module)
{
	module.AddArray("Game Objects");

	for (uint i = 0; i < list_of_gos.size(); ++i)
		list_of_gos[i]->Save(module);

	return true;
}

bool ModuleGameObjectManager::Load(JSONParser &module)
{
	// Deleting all current GameObjects;
	for (uint i = 1; i < list_of_gos.size(); ++i)
		list_of_gos[i]->to_delete = true;

	std::vector<GameObject*> list_of_gos_loaded;

	for (int i = 0; i < module.GetArrayCount("Game Objects"); ++i)
	{
		JSONParser go = module.GetArray("Game Objects", i);
		GameObject *new_go = CreateGameObject(go.GetString("Name"));

		new_go->Load(go);
		list_of_gos_loaded.push_back(new_go);
	}

	// Now, linking gameobjects.
	for (uint i = 0; i < list_of_gos_loaded.size(); ++i)
	{
		GameObject *parent = GetGameObject(list_of_gos_loaded[i]->parent_UUID, list_of_gos_loaded);
		if (parent)
		{
			list_of_gos_loaded[i]->parent = parent;   // Assigning parent
			parent->children.push_back(list_of_gos_loaded[i]);  // Assigning as child
		}
		else
		{
			list_of_gos_loaded[i]->parent = root;   // Assigning root as parent
			root->children.push_back(list_of_gos_loaded[i]);  // Assigning a child on root
		}			
	}
		
	return true;
}

void ModuleGameObjectManager::GenerateUUID(GameObject *go)
{
	go->UUID = UUID_generator.Int();
}

void ModuleGameObjectManager::GenerateUUID(Component *comp)
{
	comp->UUID = UUID_generator.Int();
}

void ModuleGameObjectManager::RayCast(const math::LineSegment &ray_cast)
{
	GameObject *curr_go = nullptr;
	GameObject *selection_canditate = nullptr;
	float min_dist = 1.0f;

	std::vector<GameObject*> selection;
	oc_tree.CollectCandidates(selection, ray_cast);
	
	for (uint i = 0; i < selection.size(); ++i)
	{
		curr_go = selection[i];
		
		math::AABB bbox; curr_go->GetAABB(bbox);
		math::Triangle tri;		

		const Mesh *mesh = curr_go->GetMesh();		
		
		if ( mesh && ray_cast.Intersects(bbox))
		{
			math::LineSegment ray_cast_transformed = ray_cast;
			ray_cast_transformed.Transform(curr_go->transform->world_transform.Inverted());

			for (uint j = 0; j < mesh->num_indices; j = j + 3)
			{
				tri.a = mesh->vertices[mesh->indices[j]];
				tri.b = mesh->vertices[mesh->indices[j + 1]],
				tri.c = mesh->vertices[mesh->indices[j + 2]];								   
				//tri.Transform(curr_go->transform->world_transform);				
				
				float hit_dist;
				math::vec hit_point;

				if (ray_cast_transformed.Intersects(tri, &hit_dist, &hit_point) && hit_dist < min_dist)
				{
					selection_canditate = curr_go;
					min_dist = hit_dist;
				}
			}
		}
	}

	App->editor->ChangeSelectedGameObject(selection_canditate);
}

void ModuleGameObjectManager::UpdateOcTree()
{
	oc_tree.Clear();
	oc_tree.SetBoundaries(oc_tree_boundaries);

	GameObject *curr_go = nullptr;

	for (uint i = 0; i < list_of_gos.size(); ++i)
	{
		curr_go = list_of_gos[i];
		if (curr_go->bstatic && curr_go->active)
			oc_tree.Insert(curr_go);
	}
}