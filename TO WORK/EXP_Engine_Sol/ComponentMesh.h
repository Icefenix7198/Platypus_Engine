#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Globals.h"
#include "Component.h"
#include "aasimp.h"

#include "Application.h"

#include <vector>
#include <string>

class ComponentMesh : public Component
{
public:
	bool active;

	ComponentMesh();
	ComponentMesh(Mesh* _mesh);
	~ComponentMesh();

	bool Update();
	void OnEditor();

	Mesh* mesh;
	bool wireMode;
private:

};

#endif //!__COMPONENT_MESH_H__
