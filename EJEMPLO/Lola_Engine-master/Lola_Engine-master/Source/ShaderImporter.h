#ifndef __SHADERIMPORTER_H__
#define __SHADERIMPORTER_H__

#include "Importer.h"

#include <string>

class ShaderImporter : public Importer
{

public:

	unsigned int static Load(const std::string &vertex_shader_asset);

};

#endif //__SHADERIMPORTER_H__

