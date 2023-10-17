#ifndef __COMPONENTPROPERTIESUI_H__
#define __COMPONENTPROPERTIESUI_H__

#include "Component.h"

#include <list>

class Module;

class ComponentPropertiesUI : public Component
{

private:

	bool interactable;
	bool draggable;
	std::list<Module*> listeners;

public:

	ComponentPropertiesUI();
	~ComponentPropertiesUI();

	void ShowEditorInfo();

	void AddListener(Module *listener);
	const std::list<Module*> &GetListeners() const;

	bool Save(JSONParser &go);
	bool Load(JSONParser &comp);
};

#endif __COMPONENTPROPERTIESUI_H__


