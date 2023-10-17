#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"

#include <vector>
#include <string>

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();
		//Component* CreateComponent(type)

	bool active;
	std::string name;
	//std::vector<Component*> components


private:

};






#endif //__GAMEOBJECT_H__