#ifndef __COMPONENTTEXTURE_H__
#define __COMPONENTTEXTURE_H__

#include "Component.h"
#include "Globals.h"
#include "Color.h"

#include "ResourceTexture.h"

class ComponentTexture : public Component
{

public:
	
	ResourceTexture *resource;
	
	ComponentTexture();
	~ComponentTexture();

	bool Update();

	void AddResource(Resource *res);

	void ShowEditorInfo();
	
	bool Save(JSONParser &go);
	bool Load(JSONParser &comp);
};

#endif __COMPONENTTEXTURE_H__

