#pragma once
#include "Module.h"
#include "Globals.h"

enum ImportType
{
	NOTYPE,
	MODEL,
	TEXTURES,
	MAXTYPES
};

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(Application* app, bool start_enabled = true);
	~ModuleFileSystem();

	bool Init() override; 
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp() override; 

	void CreateLibraryDirectories();

	bool AddPath(const char* path_or_zip);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;
	bool CreateDir(const char* dir);

	//Add archive to assets.
	ImportType GetTypeOfFullPath(const char* fullPath);
	bool AddToAssets(const char* fullPathToAdd, const char* folder);

public:

private:
	
};