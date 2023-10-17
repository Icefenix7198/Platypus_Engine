#ifndef __TEXTUREIMPORTER_H__
#define __TEXTUREIMPORTER_H__

#include "Importer.h"

#include <string>

class Resource;
class ResourceTexture;

class TextureImporter : public Importer
{

public:

	Resource static *Import(const std::string &asset_to_import, const long unsigned int &res_id);
	unsigned int static Load(const std::string &imported_file, ResourceTexture *res_mat);
	unsigned int static Save(unsigned char **data, unsigned int size);

};

#endif //__TEXTUREIMPORTER_H__

