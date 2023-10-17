#ifndef __RESOURCE_MATERIAL_H__
#define __RESOURCE_TEXTURE_H__

#include "Resource.h"

#include "MathGeoLib\MathGeoLib.h"

class ResourceMaterial : public Resource
{

public:

	math::float3  color_diffuse;
	math::float3  color_specular;
	math::float3  color_ambient;
	math::float3  color_emissive;
	math::float3  color_transparent;
	float opacity;

	ResourceMaterial(long unsigned int id, int timestamp);
};


#endif //!__RESOURCE_MATERIAL_H__
