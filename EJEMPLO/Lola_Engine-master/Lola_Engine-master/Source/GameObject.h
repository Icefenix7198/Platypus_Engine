#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"

#include "JSONParser.h"

#include <vector>

enum COMPONENT_TYPE;

class Component;
class ComponentTransform;
class ComponentTransform2D;
class ComponentPropertiesUI;

struct Mesh;

class GameObject
{

private:	

	char			 name[SHORT_STRING];	

public:

	// Unique components
	ComponentTransform				*transform = nullptr;
	ComponentTransform2D			*transform_2d = nullptr;
	ComponentPropertiesUI			*ui_properties = nullptr;

	std::vector<Component*>			 components;
	std::vector<GameObject*>         children;
	GameObject						 *parent = nullptr;

	long unsigned int				 UUID;
	long unsigned int				 parent_UUID;  // Used when loading GOs.

	bool						     active;
	bool					         selected;
	bool							 to_delete;
	bool							 bstatic;

	GameObject(const char* name);
	~GameObject();

	const Component *GetComponentByType(COMPONENT_TYPE type) const;
	const Component *AddComponent(COMPONENT_TYPE type);

	// Getters!
	bool GetAABB(math::AABB &aabb) const;
	bool GetFrustum(math::Frustum &frustum) const;
	const Mesh *GetMesh() const;
	const Mesh *GetPanel() const;

	void ChangeActiveState(bool *state, GameObject *go = nullptr);
	
	void Enable();
	void Disable();
	bool IsActive() const;
	const char *GetName() const;

	bool Save(JSONParser &game_objects);
	bool Load(JSONParser &go);
	
};

#endif //!__GAMEOBJECT_H__
