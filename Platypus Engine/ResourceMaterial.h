#pragma once
#include "Resource.h"
#include "Globals.h"
#include "Glew/include/glew.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include<string>

struct _Texture
{	
	//Texture ID
	GLuint textureBuffer = 0;

	//Texture info
	int width = 0;
	int heigth = 0;
	std::string name;

	_Texture() {};
	_Texture(std::string _name, GLuint _id, int w, int h) { name = _name, textureBuffer = _id, width = w, heigth = h; };
};

class ResourceMaterial : public Resource
{
public:
	ResourceMaterial();
	ResourceMaterial(uint32_t uid, ResourceType type);
	~ResourceMaterial();
public:
	ResourceType type;
	uint32_t UUID;
	_Texture tex;
};
