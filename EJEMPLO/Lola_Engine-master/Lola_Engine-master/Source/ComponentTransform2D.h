#ifndef __COMPONENTTRANSFORM2D_H__
#define __COMPONENTTRANSFORM2D_H__

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

class ResourceMesh;

class ComponentTransform2D : public Component
{

private:
	
	math::float3 global_position;				// World position representation
	math::float3 local_position;				// Position representation;
	math::float2 size;							// Size representation;

	bool apply_transformation = false;

	void InitPanelMesh();

public:

	ResourceMesh *panel = nullptr;

	ComponentTransform2D();
	bool Update();
	void AddResource(Resource *res);

	void ShowEditorInfo();

	void RecalcTranslations();
	void CalcGlobalTranslation();

	// Utilities
	void Move(const math::vec &movement);
	void ResizePanel();

	bool ContainsPoint(const math::float2 &point) const;
	
	// Setters / Getters
	const math::vec &GetGlobalPos() const;
	const math::vec &GetLocalPos() const;

	void SetLocalPos(const math::vec &local_pos);
	void SetSize(const math::float2 &size);

	// Save
	bool Save(JSONParser &go);
	bool Load(JSONParser &comp);
};

#endif __COMPONENTTRANSFORM2D_H__
