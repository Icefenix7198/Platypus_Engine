#include "GameObject.h"

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentCamera.h"

#include "ComponentTransform2D.h"
#include "ComponentPropertiesUI.h"
#include "ComponentImageUI.h"
#include "ComponentLabelUI.h"
#include "ComponentButtonUI.h"

#include "ResourceMesh.h"

#include "Application.h"
#include "ModuleGameObjectManager.h"

GameObject::GameObject(const char* name)
{
	active = true;
	selected = false;
	to_delete = false;
	bstatic = true;

	sprintf_s(this->name, SHORT_STRING, name);

	App->gameobject_manager->GenerateUUID(this);		
}

GameObject::~GameObject()
{
	for (uint i = 0; i < components.size(); ++i)
		RELEASE(components[i]);
	
	components.clear();
	children.clear();
}

const Component* GameObject::AddComponent(COMPONENT_TYPE type)
{
	Component *comp = nullptr;

	switch (type)
	{
	case(COMPONENT_TYPE::COMP_TRANSFORM):
		comp = new ComponentTransform();
		if(transform == nullptr)
			transform = (ComponentTransform*)comp;
		break;
	case(COMPONENT_TYPE::COMP_MESH):
		comp = new ComponentMesh();
		break;
	case(COMPONENT_TYPE::COMP_TEXTURE):
		comp = new ComponentTexture();
		break;
	case(COMPONENT_TYPE::COMP_CAMERA):
		comp = new ComponentCamera();
		break;

	// UI components
	case(COMPONENT_TYPE::COMP_TRANSFORM_2D):
		comp = new ComponentTransform2D();
		if (transform_2d == nullptr)
			transform_2d = (ComponentTransform2D*)comp;
		break;
	case(COMPONENT_TYPE::COMP_UI_PROPERTIES):
		comp = new ComponentPropertiesUI();
		if (ui_properties == nullptr)
			ui_properties = (ComponentPropertiesUI*)comp;
		break;
	case(COMPONENT_TYPE::COMP_UI_IMAGE):
		comp = new ComponentImageUI();
		break;	
	case(COMPONENT_TYPE::COMP_UI_LABEL):
		comp = new ComponentLabelUI();
		break;
	case(COMPONENT_TYPE::COMP_UI_BUTTON):
		comp = new ComponentButtonUI();
		break;
	}

	comp->game_object = this;
	components.push_back(comp);
	return comp;
}

const Component *GameObject::GetComponentByType(COMPONENT_TYPE type) const
{
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->GetType() == type)
			return components[i];
	}

	return nullptr;
}

void GameObject::ChangeActiveState(bool *state, GameObject *go)
{
	if (go == nullptr)
		go = this;

	go->active = *state;

	for (uint i = 0; i < go->children.size(); ++i)
		ChangeActiveState(state, go->children[i]);

}

void GameObject::Enable() { active = true; }
void GameObject::Disable() { active = false; }
bool GameObject::IsActive() const { return active; }
const char *GameObject::GetName() const { return name; }

bool GameObject::GetAABB(math::AABB &aabb) const
{
	bool ret = false;
	aabb.SetNegativeInfinity();
	const ComponentMesh *mesh = (ComponentMesh*)GetComponentByType(COMPONENT_TYPE::COMP_MESH);
	if (mesh)
	{
		ret = true;
		aabb = mesh->bounding_box;
	}		

	return ret;
}

bool GameObject::GetFrustum(math::Frustum &frustum) const
{
	bool ret = false;
	const ComponentCamera *camera = (ComponentCamera*)GetComponentByType(COMPONENT_TYPE::COMP_CAMERA);
	if (camera)
	{
		ret = true;
		frustum = camera->cam_frustum;
	}

	return ret;
}

const Mesh *GameObject::GetMesh() const
{
	if((ComponentMesh*)GetComponentByType(COMPONENT_TYPE::COMP_MESH) != nullptr)
		return ((ComponentMesh*)GetComponentByType(COMPONENT_TYPE::COMP_MESH))->resource->mesh_data;	
	return nullptr;
}

const Mesh *GameObject::GetPanel() const
{
	if ((ComponentTransform2D*)GetComponentByType(COMPONENT_TYPE::COMP_TRANSFORM_2D) != nullptr)
		return ((ComponentTransform2D*)GetComponentByType(COMPONENT_TYPE::COMP_TRANSFORM_2D))->panel->mesh_data;
	return nullptr;
}

 bool GameObject::Save(JSONParser &game_objects)
 {
	 if (UUID != 0) // Root will not be saved!
	 {
		 JSONParser go;

		 go.AddString("Name", name);

		 go.AddBoolean("Active", active);
		 go.AddBoolean("Selected", selected);
		 go.AddBoolean("ToDelete", to_delete);
		 go.AddBoolean("Static", bstatic);

		 go.AddUUID("UUID", UUID);
		 go.AddUUID("Parent UUID", parent->UUID);

		 go.AddArray("Components");

		 for (uint i = 0; i < components.size(); ++i)
			 components[i]->Save(go);

		 game_objects.AddArray(go);
	 }	

	 return true;
 }

 bool GameObject::Load(JSONParser &go)
 {
	 active = go.GetBoolean("Active");
	 selected = go.GetBoolean("Selected");
	 to_delete = go.GetBoolean("ToDelete");
	 bstatic = go.GetBoolean("Static");

	 UUID = go.GetUUID("UUID");
	 parent_UUID = go.GetUUID("Parent UUID");
	 
	 for (int i = 0; i < go.GetArrayCount("Components"); ++i)
	 {
		 JSONParser component = go.GetArray("Components", i);
		 Component *comp = nullptr;

		 switch ((COMPONENT_TYPE)component.GetInt("Type"))
		 {
			 case COMPONENT_TYPE::COMP_TRANSFORM: comp = (Component*)AddComponent(COMPONENT_TYPE::COMP_TRANSFORM); break;					 
			 case COMPONENT_TYPE::COMP_MESH: comp = (Component*)AddComponent(COMPONENT_TYPE::COMP_MESH); break;			 
			 case COMPONENT_TYPE::COMP_TEXTURE: comp = (Component*)AddComponent(COMPONENT_TYPE::COMP_TEXTURE); break;
			 case COMPONENT_TYPE::COMP_CAMERA: comp = (Component*)AddComponent(COMPONENT_TYPE::COMP_CAMERA); break;
		 }
		comp->Load(component);
	 }

	 return true;
 }
