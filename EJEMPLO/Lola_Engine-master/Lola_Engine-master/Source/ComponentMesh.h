#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"

#include "MathGeoLib\MathGeoLib.h"

class ResourceMesh;

class ComponentMesh : public Component
{

public:

	bool wire = false;

	math::AABB initial_bounding_box;
	math::AABB bounding_box;

	ResourceMesh *resource;

	ComponentMesh();
	~ComponentMesh();

	void AddResource(Resource *res);

	void ShowEditorInfo();

	void ApplyTransformToAABB();

	bool Save(JSONParser &go);
	bool Load(JSONParser &comp);
};

#endif __COMPONENTMESH_H__
