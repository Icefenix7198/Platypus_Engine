
#include "ResourceScene.h"

ResourceScene::ResourceScene(long unsigned int id, int timestamp) : Resource()
{
	this->id = id;
	this->timestamp = timestamp;
	type = RESOURCE_TYPE::RES_SCENES;
}


