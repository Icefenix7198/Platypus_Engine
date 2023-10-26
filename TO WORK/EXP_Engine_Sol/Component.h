#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Globals.h"
#include "GameObject.h"

#include <vector>
#include <string>


enum ComponentType
{
	TRANSFORM,
	MESH,
	MATERIAL,
};

class Component
{
public:
	Component();
	~Component();

	ComponentType type;
	bool active;
	GameObject* owner;
	
	virtual bool Enable();
	virtual bool Update();
	virtual bool Disable();

	virtual void OnEditor();

private:

};

#endif //!__COMPONENT_H__
