#include "Application.h"
#include "ModuleFileSystem.h"
#include "Globals.h"

#include "SDL\include\SDL.h"
#include "PhysFS\include\physfs.h" //Works better than direct.h

#pragma comment (lib, "PhysFS/libx86/physfs.lib")




ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(nullptr);
	SDL_free(base_path);

	//Setting the working directory as the writing directory
	if (PHYSFS_setWriteDir(".") == 0)
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	AddPath("."); //Adding ProjectFolder (working directory)
	AddPath("Assets");
	CreateLibraryDirectories();
}

// Destructor
ModuleFileSystem::~ModuleFileSystem()
{
	PHYSFS_deinit();
}

// Called before render is available
bool ModuleFileSystem::Init()
{
	LOG("Loading File System");
	bool ret = true;

	// Ask SDL for a write dir
	char* write_path = SDL_GetPrefPath("UPC", "Platypus Engine"); //Lo hacen con esto desde la AppEngine->GetOrganizationName(), Engine->GetTitleName() que simplemente retorna unas constants

	// Trun this on while in game mode
	//if(PHYSFS_setWriteDir(write_path) == 0)
	//	LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	SDL_free(write_path);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleFileSystem::PreUpdate(float dt)
{


	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleFileSystem::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleFileSystem::CleanUp()
{

	return true;
}

void ModuleFileSystem::CreateLibraryDirectories()
{
	CreateDir(LIBRARY_PATH);
	CreateDir(FOLDERS_PATH);
	CreateDir(MESHES_PATH);
	CreateDir(TEXTURES_PATH);
	CreateDir(MATERIALS_PATH);
	CreateDir(MODELS_PATH);
	CreateDir(ANIMATIONS_PATH);
	CreateDir(PARTICLES_PATH);
	CreateDir(SHADERS_PATH);
	CreateDir(MAPS_PATH);
}

bool ModuleFileSystem::AddPath(const char* path_or_zip)
{
	bool ret = false;

	if (PHYSFS_mount(path_or_zip, nullptr, 1) == 0)
	{
		LOG("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	}
	else
		ret = true;

	return ret;
}

//Check if exists
bool ModuleFileSystem::Exists(const char* file) const
{
	return PHYSFS_exists(file) != 0;
}

bool ModuleFileSystem::CreateDir(const char* dir)
{
	if (IsDirectory(dir) == false)
	{
		PHYSFS_mkdir(dir);
		return true;
	}
	return false;
}

ImportType ModuleFileSystem::GetTypeOfFullPath(const char* fullPath)
{
	ImportType ret = ImportType::NOTYPE;

	std::string pathFile;
	std::string fileType;
	pathFile.assign(fullPath);
	bool pointFound = false;
	//We search were the point is in the string to select the fileType
	for (int i = 0; i < pathFile.size(); i++)
	{
		if (pointFound)
		{
			//Pushback the chars after the point (they will be the file tipe) ex: .fbx, .png, etc.
			fileType.push_back(pathFile.at(i));
		}
		if (pathFile.at(i) == '.')
		{
			pointFound = true;
		}
	}

	if (std::strcmp(fileType.c_str(), "FBX") == 0 || (std::strcmp(fileType.c_str(), "fbx") == 0))
	{
		ret = ImportType::MODEL;
	}

	if (std::strcmp(fileType.c_str(), "png") == 0 || (std::strcmp(fileType.c_str(), "dds") == 0) || (std::strcmp(fileType.c_str(), "DDS") == 0))
	{
		ret = ImportType::TEXTURES;
	}

	return ret;
}

bool ModuleFileSystem::AddToAssets(const char* fullPathToAdd, const char* folder)
{
	bool ret = false;

	//Fopen to be able to read information Documentation :https://learn.microsoft.com/es-es/cpp/c-runtime-library/reference/fopen-s-wfopen-s?view=msvc-170
	std::FILE* pFile;
	fopen_s(&pFile, fullPathToAdd, "rb"); //Pointer to the file to access, name of the file, mode of acces, rb becose reasons

	//Get pointer
	PHYSFS_file* dest = PHYSFS_openWrite(folder);

	uint size = 0;

	if (pFile != nullptr)
	{
		fseek(pFile, 0, SEEK_END);
		size = ftell(pFile);
		
	}
	else
	{
		LOG("FILE SYSTEM: Could not open file '%s' to read", fullPathToAdd);
	}
		

	return ret;
}

// Check if a file is a directory
bool ModuleFileSystem::IsDirectory(const char* file) const
{
	return PHYSFS_isDirectory(file) != 0;
}