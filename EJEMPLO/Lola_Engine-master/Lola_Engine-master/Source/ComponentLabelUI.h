#ifndef __COMPONENTTLABELUI_H__
#define __COMPONENTTLABELUI_H__

#include "Component.h"

class ResourceTexture;

class ComponentLabelUI : public Component
{

public:

	ResourceTexture *resource;

	ComponentLabelUI();
	~ComponentLabelUI();

	void AddResource(Resource *res);
	void SetText();

	void ShowEditorInfo();

	bool Save(JSONParser &go);
	bool Load(JSONParser &comp);
};

#endif __COMPONENTTLABELUI_H__


