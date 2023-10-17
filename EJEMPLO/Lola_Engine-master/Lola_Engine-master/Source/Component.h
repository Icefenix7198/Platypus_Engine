#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "JSONParser.h"

#include "Application.h"
#include "ModuleResourceManager.h"

class GameObject;
class Resource;

enum COMPONENT_TYPE
{
	COMP_UNKNOWN,

	// Generic components
	COMP_TRANSFORM, 
	COMP_MATERIAL,
	COMP_TEXTURE,
	COMP_MESH,
	COMP_CAMERA,

	// UI components
	COMP_TRANSFORM_2D,
	COMP_UI_PROPERTIES,
	COMP_UI_IMAGE,
	COMP_UI_LABEL, 
	COMP_UI_BUTTON
};

class Component
{

protected:

	const char				 *name;
	COMPONENT_TYPE            type;
	bool					active;	

public:
	
	GameObject *game_object = nullptr;
	long unsigned int	      UUID;

	Component();
	virtual ~Component();

	virtual bool Update();
	virtual void ShowEditorInfo();

	COMPONENT_TYPE &GetType();
	const char *GetName();
	bool IsActive() const;

	virtual void AddResource(Resource *res)
	{ }

	virtual bool Save(JSONParser &go);
	virtual bool Load(JSONParser &component);
};

#endif //!__COMPONENT_H__
