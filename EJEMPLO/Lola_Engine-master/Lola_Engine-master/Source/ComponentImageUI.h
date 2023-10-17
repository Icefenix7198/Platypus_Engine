#ifndef __COMPONENTTIAMGEUI_H__
#define __COMPONENTTIAMGEUI_H__

#include "Component.h"

class ResourceTexture;

class ComponentImageUI : public Component
{

public:

	ResourceTexture *resource;

	ComponentImageUI();
	~ComponentImageUI();

	void AddResource(Resource *res);
	void InitTexture(const char *tex_name);

	void ShowEditorInfo();

	bool Save(JSONParser &go);
	bool Load(JSONParser &comp);
};

#endif __COMPONENTTIAMGEUI_H__

