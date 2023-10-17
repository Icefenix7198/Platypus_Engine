#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Component.h"

#include "MathGeoLib\MathGeoLib.h"

class ComponentCamera : public Component
{

private:

	bool camera_modified;

	float aspect_ratio;
	float FOV_h; // It's not used!
	float FOV_v;
	float near_plane, far_plane;

	float near_plane_tmp, far_plane_tmp;

	float max_far_plane, min_near_plane;

public:

	bool editor_camera;

	math::Frustum cam_frustum;
	math::Frustum origin_cam_frustum;

	ComponentCamera();
	~ComponentCamera();
	
	bool Update();
	void SetComponent();

	void ShowEditorInfo();
	void UpdateFrustum();
	void ApplyTransformToFrustum();
	void AddResource(const Resource *res) {}

	bool Save(JSONParser &go);
	bool Load(JSONParser &comp);

};

#endif __COMPONENTTRANSFORM_H__

