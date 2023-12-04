#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

#include "Globals.h"
#include "GameObject.h"

#include "Component.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera();
	ComponentCamera(GameObject* own);
	~ComponentCamera();
};

#endif //!__COMPONENT_CAMERA_H__

