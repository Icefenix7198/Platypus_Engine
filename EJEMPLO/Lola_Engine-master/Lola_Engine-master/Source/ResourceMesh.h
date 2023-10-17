#ifndef __RESOURCE_MESH_H__
#define __RESOURCE_MESH_H__

#include "Resource.h"

struct Mesh;

class ResourceMesh : public Resource
{

public:

	Mesh *mesh_data;

	ResourceMesh(long unsigned int id, int timestamp);
	~ResourceMesh();

	bool LoadToMemory();
	bool UnloadFromMemory();
};


#endif //!__RESOURCE_MESH_H__

