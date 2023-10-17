#ifndef __MODULEFILESYSTEM_H__
#define __MODULEFILESYSTEM_H__

#include "Module.h"

#include "Assimp\include\cfileio.h"
#include "Devil\include\il.h"

// Library folders
#define LIBRARY "Library/"
#define LIBRARY_TEXTURE LIBRARY "Textures/"
#define LIBRARY_MESH LIBRARY "Mesh/"
#define LIBRARY_SCENES LIBRARY "Scenes/"
#define LIBRARY_MATERIAL LIBRARY "Material/"

// Asset folder
#define ASSETS "Assets/"

struct SDL_RWops;
int close_sdl_rwops(SDL_RWops *rw);

class ModuleFileSystem : public Module
{
public:
	
	ModuleFileSystem(Application* app, bool start_enabled = true);
	~ModuleFileSystem();

	bool Awake(JSONParser &config);
	bool CleanUp();

	bool AddSearchPath(const char *path_or_zip, const char *mount_point = nullptr);
	bool RemoveAllSearchPaths();
	bool RemovePath(const char *path_or_zip);

	bool RemoveFile(const char *file);

	uint Load(const char* file, char **buffer) const;
	SDL_RWops *Load(const char* file) const;
	uint Save(const char *file, const char *buffer, uint size) const;

	const char *GetFileFromDirPath(const char *path) const;
	const char *GetSaveDirectory() const;
	const char *GetRealDirectory(const char* file) const;
	int GetLastTimeMod(const char *file, const char *dir = nullptr) const;
	const char *GetWorkingDirectory() const;

	bool IsDirectory(const char *dir) const;
	bool Exists(const char *file) const;

	void ExploreFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list, bool recursive = false) const;

	void SetWriteDirectory();

	aiFileIO * ModuleFileSystem::GetAssimpIO();

private:

	aiFileIO *AssimpIO = nullptr;	
	void ModuleFileSystem::CreateAssimpIO();
	void ModuleFileSystem::SetDevilIO();	
};


#endif //!__MODULEFILESYSTEM_H__