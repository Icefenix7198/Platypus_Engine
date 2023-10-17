#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include "Importer.h"

#include <string>

struct Mesh;
struct aiMesh;
class Resource;
class ResourceMesh;

class MeshImporter : public Importer
{
public:

	Resource static *Import(const aiMesh *ai_mesh, const long unsigned int &res_id);

	unsigned int static Save(char **data, const Mesh *mesh);

	unsigned int static Load(const std::string &imported_file, ResourceMesh *mesh);
	void static Load(const aiMesh *ai_mesh, Mesh *mesh);
	
};

#endif // !__MESHIMPORTER_H__
