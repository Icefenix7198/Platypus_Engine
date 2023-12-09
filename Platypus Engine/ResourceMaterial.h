#pragma once
#include "Resource.h"
#include "Globals.h"
#include "Glew/include/glew.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include<string>

class ResourceMaterial : public Resource
{
public:
	ResourceMaterial();
	ResourceMaterial(uint32_t uid, ResourceType type);
	~ResourceMaterial();
public:
	ResourceType type;
	uint32_t UUID;
};
