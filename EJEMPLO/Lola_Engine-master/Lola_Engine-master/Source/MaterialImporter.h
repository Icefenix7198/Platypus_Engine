#ifndef __MATERIALIMPORTER_H__
#define __MATERIALIMPORTER_H__

#include "Importer.h"

#include <string>

struct aiMaterial;
class Resource;
class ResourceMaterial;

class MaterialImporter : public Importer
{

public:

	Resource static *Import(const aiMaterial *ai_material, const long unsigned int &res_id);
	unsigned int static Load(const std::string &imported_file, ResourceMaterial *res_mat);

};

#endif //__MATERIALIMPORTER_H__

