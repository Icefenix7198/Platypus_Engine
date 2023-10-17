#include "ResourceMaterial.h"

ResourceMaterial::ResourceMaterial(long unsigned int id, int timestamp) : Resource()
{
	this->id = id;
	this->timestamp = timestamp;
	type = RESOURCE_TYPE::RES_MATERIAL;

	color_diffuse = math::float3::zero;
	color_specular = math::float3::zero;
	color_ambient = math::float3::zero;
	color_emissive = math::float3::zero;
	color_transparent = math::float3::zero;

	opacity = 0;
}