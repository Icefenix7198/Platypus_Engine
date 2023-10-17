#include "ModuleFileSystem.h"

#include "Application.h"

#include "SDL\include\SDL.h"
#include "PhysFS\include\physfs.h"

#pragma comment (lib, "Source/PhysFS/libx86/physfs.lib")

ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "File System");

	// PHYSFS must be initialized before other modules awake,
	// because it will be used by them.
	if (PHYSFS_isInit() == 0)
	{
		char *base_path = SDL_GetBasePath();
		PHYSFS_init(base_path);
		SDL_free(base_path);

		AddSearchPath(".");
	}	

}

ModuleFileSystem::~ModuleFileSystem()
{
	if (PHYSFS_isInit() != 0)
		PHYSFS_deinit();
}

bool ModuleFileSystem::Awake(JSONParser &config)
{
	bool ret = true;

	SetWriteDirectory();

	// Generate IO interfaces
	CreateAssimpIO();
	//SetDevilIO();
	
	return ret;
}

bool ModuleFileSystem::CleanUp()
{
	//RemoveAllSearchPaths();
	//RELEASE(AssimpIO);
	delete AssimpIO;

	return true;
}

bool ModuleFileSystem::AddSearchPath(const char *path_or_zip, const char *mount_point)
{
	bool ret = true;

	// CRZ addition!
	if (PHYSFS_exists(path_or_zip))
	{
		if(PHYSFS_removeFromSearchPath(path_or_zip))
			DEBUG("Failure on removing path %s for seach path. Error: %s", path_or_zip, PHYSFS_getLastError());
	}

	if (PHYSFS_mount(path_or_zip, mount_point, 1) == 0)
	{
		DEBUG("%s %s", "Failure on mounting or adding path", path_or_zip);
		DEBUG("%s", "Error:", PHYSFS_getLastError());
		ret = false;
	}

	return ret;
}

bool ModuleFileSystem::RemoveAllSearchPaths()
{
	bool ret = false;

	char **paths;
	for (paths = PHYSFS_getSearchPath(); *paths != NULL; ++paths)
	{
		PHYSFS_removeFromSearchPath(*paths);
	}

	if (*(paths = PHYSFS_getSearchPath()) == NULL)
		ret = true;

	PHYSFS_freeList(paths);	

	return ret;
}

bool ModuleFileSystem::RemovePath(const char *path_or_zip)
{
	bool ret = true;
		
	if (PHYSFS_removeFromSearchPath(path_or_zip) == 0)
	{
		DEBUG("%s %s", "Failure on removing directory or file on search path", path_or_zip);
		DEBUG("%s", "Error:", PHYSFS_getLastError());
		ret = false;
	}

	return ret;
}

bool ModuleFileSystem::RemoveFile(const char *file_name)
{
	if (PHYSFS_delete(file_name) == 0)
	{
		DEBUG("Error while deleting %s. %s", file_name, PHYSFS_getLastError());
		return false;
	}
	return true;		
}

uint ModuleFileSystem::Load(const char* file, char **buffer) const
{
	uint ret = 0;

	PHYSFS_file *file_handle = PHYSFS_openRead(file);
	if (file_handle != NULL)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(file_handle);
		if (size > 0)
		{
			*buffer = new char[(uint)size];
			PHYSFS_sint64 bytes_readed = PHYSFS_read(file_handle, *buffer, 1, size);
			if (bytes_readed != size)
			{
				DEBUG("File system error while reading from file %s: %s", file, PHYSFS_getLastError());
				if (*buffer != nullptr)
					delete[] *buffer;
			}
			else
				ret = (uint)size;
		}

		if (PHYSFS_close(file_handle) == 0)
			DEBUG("File %s is not closed properly. Error: %s", file, PHYSFS_getLastError());
	}

	return ret;
}

SDL_RWops *ModuleFileSystem::Load(const char *file) const
{
	SDL_RWops *ret = NULL;
	char *buffer;
	uint size = Load(file, &buffer);

	if (size > 0)
	{
		ret = SDL_RWFromConstMem(buffer, size);
		if (ret != NULL)
			ret->close = close_sdl_rwops;
	}
	
	return ret;
}

int close_sdl_rwops(SDL_RWops *rw)
{
	if (rw->hidden.mem.base != nullptr)
		delete rw->hidden.mem.base;
	SDL_FreeRW(rw);
	return 0;
}

uint ModuleFileSystem::Save(const char *file, const char *buffer, uint size) const
{
	uint ret = 0;

	PHYSFS_file *file_handle = PHYSFS_openWrite(file);
	if (file_handle != NULL)
	{
		PHYSFS_sint64 bytes_written = PHYSFS_write(file_handle, (const void*) buffer, 1, size);
		if (bytes_written != size)
		{
			DEBUG("Failure on writing %s. Error: %s", file, PHYSFS_getLastError());
		}			
		else
			ret = (uint)size;

		if (PHYSFS_close(file_handle) == 0)
			DEBUG("File System can not close file %s. Error: %s", file, PHYSFS_getLastError());
	}
	else
		DEBUG("File System failure while opening file %s. Error: %s", file, PHYSFS_getLastError());
	
	return ret;
}

bool ModuleFileSystem::IsDirectory(const char *dir) const
{
	return (PHYSFS_isDirectory(dir) == 0) ? false : true;
}

bool ModuleFileSystem::Exists(const char *file) const
{
	return (PHYSFS_exists(file) == 0) ? false : true;
}

const char *ModuleFileSystem::GetSaveDirectory() const
{
	return "Save/";
}

const char *ModuleFileSystem::GetFileFromDirPath(const char *path) const
{
	char *file = '\0';  // Empty file string 
	const char *cursor = path;

	if (cursor != nullptr)
	{
		// We search for slash last position "\"
		while (*(cursor++) != '\0')
		{
			if (*cursor == '\\')
				file = (char*)cursor;
		}
	};	

	return file == '\0' ? path : ++file;
}

const char *ModuleFileSystem::GetRealDirectory(const char *file) const
{
	return PHYSFS_getRealDir(file);
}

int ModuleFileSystem::GetLastTimeMod(const char *file, const char *dir) const
{
	char path[SHORT_STRING];
	if(dir)
		sprintf_s(path, SHORT_STRING, "%s%s", dir, file);
	else
		sprintf_s(path, SHORT_STRING, file);
	
	return PHYSFS_getLastModTime(path);
}

const char *ModuleFileSystem::GetWorkingDirectory() const
{
	return SDL_GetBasePath();
}

void ModuleFileSystem::ExploreFiles(const char* from_directory, std::vector<std::string> &file_list, std::vector<std::string> &directory_list, bool recursive) const
{
	char **rc = PHYSFS_enumerateFiles(from_directory);
	char **i;

	//std::string dir(directory);

	for (i = rc; *i != nullptr; i++)
	{
		// Ricard's code-> (dir + *i).c_str() instead of *i on isDirectory();
		if (PHYSFS_isDirectory(*i))
		{
			directory_list.push_back(*i);
			if(recursive)
			{
				std::string dir(from_directory);
				ExploreFiles((dir + "/" + *i).c_str(), file_list, directory_list, recursive);
			}
		}						
		else
			file_list.push_back(*i);
	}

	PHYSFS_freeList(rc);
}

void ModuleFileSystem::SetWriteDirectory()
{
	switch (App->GetEngineMode())
	{
		case(ENGINE_MODE::EDITOR):
		{
			char write_dir[MEDIUM_STRING];
			sprintf_s(write_dir, MEDIUM_STRING, "%s%s", PHYSFS_getBaseDir(), "Game");	
			//sprintf_s(write_dir, MEDIUM_STRING, "%s", PHYSFS_getBaseDir());

			if (PHYSFS_setWriteDir(write_dir) == 0)
				DEBUG("%s,%s", "Error on setting Write Directory. Error:", PHYSFS_getLastError());
			else
			{				
				AddSearchPath("Assets/Models", "Models");
				AddSearchPath("Assets/Textures", "Textures");
				AddSearchPath("Assets/Shaders/Vertex", "VertexShaders");
				AddSearchPath("Assets/Shaders/Fragment", "FragmentShaders");

				// Creating internal folders for own engine usage
				if (!PHYSFS_exists(LIBRARY_SCENES))
					PHYSFS_mkdir(LIBRARY_SCENES);

				if (!PHYSFS_exists(LIBRARY_TEXTURE))
					PHYSFS_mkdir(LIBRARY_TEXTURE);

				if (!PHYSFS_exists(LIBRARY_MESH))
					PHYSFS_mkdir(LIBRARY_MESH);

				if (!PHYSFS_exists(LIBRARY_MATERIAL))
					PHYSFS_mkdir(LIBRARY_MATERIAL);

				DEBUG("%s %s", "On Editor Mode, write directory is ", write_dir);
				AddSearchPath(write_dir, ".");
			}

			SDL_free(write_dir);
			break;
		}
		case(ENGINE_MODE::GAME):
		{
			char *write_dir = SDL_GetPrefPath(App->GetAppName(), App->GetOrganization());
			if (PHYSFS_setWriteDir(write_dir) == 0)
				DEBUG("%s,%s", "Error on setting Write Dir. Error:", PHYSFS_getLastError());
			else
			{
				DEBUG("%s %s", "On Game Mode, write directory is ", write_dir);
				AddSearchPath(write_dir, GetSaveDirectory());
			}
			SDL_free(write_dir);
			break;
		}
	}
}

// -----------------------------------------------------
// ASSIMP IO
// -----------------------------------------------------

size_t AssimpWrite(aiFile* file, const char* data, size_t size, size_t chunks)
{
	PHYSFS_sint64 ret = PHYSFS_write((PHYSFS_File*)file->UserData, (void*)data, size, chunks);
	if (ret == -1)
		DEBUG("File System error while WRITE via assimp: %s", PHYSFS_getLastError());

	return (size_t)ret;
}

size_t AssimpRead(aiFile* file, char* data, size_t size, size_t chunks)
{
	PHYSFS_sint64 ret = PHYSFS_read((PHYSFS_File*)file->UserData, (void*)data, size, chunks);
	if (ret == -1)
		DEBUG("File System error while READ via assimp: %s", PHYSFS_getLastError());

	return (size_t)ret;
}

size_t AssimpTell(aiFile* file)
{
	PHYSFS_sint64 ret = PHYSFS_tell((PHYSFS_File*)file->UserData);
	if (ret == -1)
		DEBUG("File System error while TELL via assimp: %s", PHYSFS_getLastError());

	return (size_t)ret;
}

size_t AssimpSize(aiFile* file)
{
	PHYSFS_sint64 ret = PHYSFS_fileLength((PHYSFS_File*)file->UserData);
	if (ret == -1)
		DEBUG("File System error while SIZE via assimp: %s", PHYSFS_getLastError());

	return (size_t)ret;
}

void AssimpFlush(aiFile* file)
{
	if (PHYSFS_flush((PHYSFS_File*)file->UserData) == 0)
		DEBUG("File System error while FLUSH via assimp: %s", PHYSFS_getLastError());
}

aiReturn AssimpSeek(aiFile* file, size_t pos, aiOrigin from)
{
	int res = 0;

	switch (from)
	{
	case aiOrigin_SET:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, pos);
		break;
	case aiOrigin_CUR:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, PHYSFS_tell((PHYSFS_File*)file->UserData) + pos);
		break;
	case aiOrigin_END:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, PHYSFS_fileLength((PHYSFS_File*)file->UserData) + pos);
		break;
	}

	if (res == 0)
		DEBUG("File System error while SEEK via assimp: %s", PHYSFS_getLastError());

	return (res != 0) ? aiReturn_SUCCESS : aiReturn_FAILURE;
}

aiFile* AssimpOpen(aiFileIO* io, const char* name, const char* format)
{
	static aiFile file;

	file.UserData = (char*)PHYSFS_openRead(name);
	file.ReadProc = AssimpRead;
	file.WriteProc = AssimpWrite;
	file.TellProc = AssimpTell;
	file.FileSizeProc = AssimpSize;
	file.FlushProc = AssimpFlush;
	file.SeekProc = AssimpSeek;

	return &file;
}

void AssimpClose(aiFileIO* io, aiFile* file)
{
	if (PHYSFS_close((PHYSFS_File*)file->UserData) == 0)
		DEBUG("File System error while CLOSE via assimp: %s", PHYSFS_getLastError());
}

void ModuleFileSystem::CreateAssimpIO()
{
	if (AssimpIO)
	{
		delete AssimpIO;
		AssimpIO = nullptr;
	}

	AssimpIO = new aiFileIO;
	AssimpIO->OpenProc = AssimpOpen;
	AssimpIO->CloseProc = AssimpClose;
}

aiFileIO * ModuleFileSystem::GetAssimpIO()
{
	return AssimpIO;
}

// -----------------------------------------------------
// DEVIL IO
// -----------------------------------------------------

//fOpenProc
//This function will open a file and by the char* parameter it receives and return an ILHANDLE to the file stream.
ILHANDLE DevilOpen2(char *file_name)
{
	PHYSFS_file *file_handle = PHYSFS_openRead(file_name);
	if (file_handle == NULL)
		DEBUG("Nein!");
		
	return (PHYSFS_File*)file_handle;
}

//fCloseProc:
//This function closes the ILHANDLE file stream passed as its parameter.
void DevilClose(ILHANDLE file_handle)
{
	if (file_handle)
	{
		if (PHYSFS_close((PHYSFS_File*)file_handle) == 0)
			DEBUG("File handler is not closed properly. Error: %s", PHYSFS_getLastError());
	}	
}

//fEofProc
//Pointer to a function that returns IL_TRUE if the end of file is reached
ILboolean DevilEof(ILHANDLE file_handler)
{
	if(file_handler)
		return (ILboolean) ( PHYSFS_tell((PHYSFS_File*)file_handler) == PHYSFS_fileLength((PHYSFS_File*)file_handler) );
	return IL_FALSE;
}

//fGetcProc:
//This function gets exactly one character from the ILHANDLE passed as its parameter.
//char *DevilGet(ILHANDLE)
//{
//
//}


//fReadProc:
//This function retrieves a specified number of bytes from the file handle passed to it.It s parameters are(in order) :
//void*, ILint, ILint, ILHANDLE.
//The void* is the buffer that receives the bytes from the file.The first ILint is the size of the data type.
//The second ILint is the number of the data types to retrieve.You can just multiply the ILints together to determine
//the number of bytes to retrieve.The ILHANDLE is the file stream to get the data from.

//fSeekProc:
//This function skips around in the file stream(setting the file pointer to a different position). It is the equivalent of fseek.
//The parameters are : ILHANDLE, ILint, ILint.The ILHANDLE is the file stream to seek in.The first ILint is the number of bytes
//to skip.The second ILint is the mode to skip with.It uses the C stdio #define's for this: SEEK_SET, SEEK_CUR, SEEK_END. You may
//need to #define these in your application if they are not already there (e.g. Visual Basic and Delphi users).
int DevilSeek(ILHANDLE file_handler, ILint pos, ILint mode)
{
	int res = 0;

	if (file_handler)
	{
		switch (mode)
		{
		case SEEK_SET:
			res = PHYSFS_seek((PHYSFS_File*)file_handler, pos);
			break;
		case SEEK_CUR:
			res = PHYSFS_seek((PHYSFS_File*)file_handler, PHYSFS_tell((PHYSFS_File*)file_handler) + pos);
			break;
		case SEEK_END:
			res = PHYSFS_seek((PHYSFS_File*)file_handler, PHYSFS_fileLength((PHYSFS_File*)file_handler) + pos);
			break;
		}
	}

	if (res == 0)
		DEBUG("File System error while SEEK via assimp: %s", PHYSFS_getLastError());

	return res;	
}
//fTellProc:
//This function reports the current position of the file stream's file pointer. It is similar to ftell. 
//The only parameter passed to this function is the ILHANDLE to determine the file pointer from
size_t DevilTell(ILHANDLE file_handler)
{
	PHYSFS_sint64 ret = -1;
	if (file_handler)
	{
		ret = PHYSFS_tell((PHYSFS_File*)file_handler);
		if (ret == -1)
			DEBUG("File System error while TELL via assimp: %s", PHYSFS_getLastError());
	}	

	return (size_t)ret;
}

void ModuleFileSystem::SetDevilIO()
{
	ilSetRead((fOpenRProc)DevilOpen2, (fCloseRProc)DevilClose, (fEofProc)DevilEof,
		(fGetcProc)DevilSeek, (fReadProc)DevilSeek, (fSeekRProc)DevilSeek, (fTellRProc)DevilTell);
	
}

