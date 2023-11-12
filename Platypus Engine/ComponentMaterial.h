#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Globals.h"
#include "GameObject.h"
#include "Component.h"

#include <vector>
#include <string>
#include "Color.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	ComponentMaterial(GameObject* own);
	~ComponentMaterial();

	void OnEditor();

	bool Update();

	bool checkers;
	bool activeCheck;

	uint textureBuffer;

	Color color{ 1,1,1,1 };

private:

};

#endif //!__COMPONENT_MATERIAL_H__
