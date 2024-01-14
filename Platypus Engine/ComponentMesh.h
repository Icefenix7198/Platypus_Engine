#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Globals.h"
#include "Component.h"
#include "ResourceMesh.h"

#include "Application.h"

#include <vector>
#include <string>

class ComponentMesh : public Component
{
public:
	bool active;

	ComponentMesh();
	ComponentMesh(GameObject* own);
	~ComponentMesh();

	AABB GenerateLocalAABB();

	AABB GetGlobalAABB();

	bool Update();
	bool GetActive();
	bool DrawVertexNormals();
	bool DrawFaceNormals();
	bool DrawGlobalAABB();
	bool DrawOBB();
	
	void OnEditor();

	uint32_t IDResourceMesh;
	ResourceMesh* resourceMesh;
	bool wireMode;
	bool drawVertexNormals;
	bool drawFaceNormals;
	bool drawAABB;
	bool drawOBB;

private:
	AABB localAABB;
	OBB mOBB;
	AABB globalAABB;
};

#endif //!__COMPONENT_MESH_H__
