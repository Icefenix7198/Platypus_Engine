#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

#include "Globals.h"
#include "GameObject.h"

#include"MathGeoLib/include/Geometry/Frustum.h"

#include "Component.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera();
	ComponentCamera(GameObject* own);
	~ComponentCamera();

	void OnEditor();

	bool Update();

	void StartCamera();
	void DrawCameraLines();
	bool ContainsAaBox(AABB& refBox);

	Frustum frustum;
	float nearDistance;
	float farDistance;
	float fov;
};

#endif //!__COMPONENT_CAMERA_H__

