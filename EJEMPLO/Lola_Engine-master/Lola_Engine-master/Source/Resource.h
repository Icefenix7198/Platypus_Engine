#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <string>

enum RESOURCE_TYPE
{
	RES_NONE,
	RES_TEXTURES, 
	RES_MATERIAL,
	RES_MESHES, 
	RES_SCENES
};

class Resource
{

protected:

	bool loaded_in_memory;
	int times_referenced;

public:

	RESOURCE_TYPE type;
	long unsigned int id;
	std::string file;
	std::string imported_file;
	int timestamp;

	Resource();
	virtual ~Resource();

	virtual bool LoadToMemory();
	virtual bool UnloadFromMemory();

	bool LoadedInMemory() const;

	int GetNumReferences() const;
	void SetNumReferences(int times);
	void IncrReferences();
	void DecrReferences();
};

#endif __RESOURCES_H__
