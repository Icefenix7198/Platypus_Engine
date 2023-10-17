#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__

#include "Resource.h"

#include "Globals.h"

#include "MathGeoLib\MathGeoLib.h"

class ResourceTexture : public Resource
{

public:

	char tex_path[SHORT_STRING];
	unsigned int tex_buffer;
	unsigned int texture_size;
	char *texture_data;	

	math::float3  color_diffuse;
	math::float3  color_specular;
	math::float3  color_ambient;
	math::float3  color_emissive;
	math::float3  color_transparent;
	float opacity;
	
	ResourceTexture(long unsigned int id, int timestamp);
	~ResourceTexture();

	bool LoadToMemory();
	bool UnloadFromMemory();
};


#endif //!__RESOURCE_MATERIAL_H__
