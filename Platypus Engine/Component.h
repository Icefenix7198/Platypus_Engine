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
	CAMERA,
	PARTICLE,
};

class Component
{
public:
	Component();
	~Component();

	//UUID
	uint32_t UUID; //Aqui hay que ponerlo en publico que sino no lo heredara bien
	uint32_t GetUUID() { return UUID; };
	void SetUUID(uint32_t ID) { UUID = ID; };

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
