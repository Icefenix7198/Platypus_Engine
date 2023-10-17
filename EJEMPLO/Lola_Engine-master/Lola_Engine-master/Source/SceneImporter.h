#ifndef __SCENEIMPORTER_H__
#define __SCENEIMPORTER_H__

#include "Importer.h"

#include <string>

class Resource;

class SceneImporter : public Importer
{
public:

	Resource static *Import(const std::string &asset_to_import, const long unsigned int &res_id);
	//bool static Load(const std::string &imported_file, ResourceScene *scene);
};


#endif //__SCENEIMPORTER_H__
