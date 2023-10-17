#ifndef __COMPONENTTBUTTONUI_H__
#define __COMPONENTTBUTTONUI_H__

#include "Component.h"

class ResourceTexture;

class ComponentButtonUI : public Component
{

public:

	ResourceTexture *current_state;

	ResourceTexture *idle;
	ResourceTexture *pushed;
	ResourceTexture *hover;

	ComponentButtonUI();
	~ComponentButtonUI();

	void InitIdleTexture(const char *tex_name);
	void InitPushedTexture(const char *tex_name);
	void InitHoverTexture(const char *tex_name);

	void ShowEditorInfo();

	bool Save(JSONParser &go);
	bool Load(JSONParser &comp);
};

#endif __COMPONENTTBUTTONUI_H__