#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Globals.h"
#include "Component.h"
#include "aasimp.h"

#include <vector>
#include <string>

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	ComponentMesh(Mesh* _mesh);
	~ComponentMesh();

	Mesh* mesh;

private:

};

#endif //!__COMPONENT_MESH_H__
