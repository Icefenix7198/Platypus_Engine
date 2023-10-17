#ifndef __APPLICATION_H__
#define __APPLICATION_H__

//#include "Globals.h"
#include "Module.h"

#include "Performance.h"

#include "DebugMode.h"

#include <list>

struct Console;

enum ENGINE_MODE
{
	EDITOR,
	GAME
};

class ModuleWindow;
class ModuleFileSystem;
class ModuleInput;
class ModuleAudio;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCameraEditor;
class ModuleEditor;
class ModuleFontManager;
class ModuleUIManager;
class ModuleGameObjectManager;
class ModuleResourceManager;
class ModuleRenderer3D;

class Application
{
public:

	Performance				perf_info;
	Console					*console = nullptr;

	ModuleWindow			*window = nullptr;
	ModuleFileSystem		*file_system = nullptr;
	ModuleInput				*input = nullptr;
	ModuleAudio				*audio = nullptr;
	ModuleSceneIntro		*scene_intro = nullptr;
	ModuleRenderer3D		*renderer3D = nullptr;
	ModuleCameraEditor		*camera = nullptr;
	ModuleGameObjectManager *gameobject_manager = nullptr;
	ModuleResourceManager   *resource_manager = nullptr;
	ModuleEditor			*editor = nullptr;
	ModuleUIManager			*ui_manager = nullptr;
	ModuleFontManager		*font_manager = nullptr;

	Application();
	~Application();

	bool Init();
	UPDATE_STATUS Update();
	bool CleanUp();

	void CloseApp();
	void RequestBrowser(const char *web_adress);

	const char *GetAppName();
	const char *GetOrganization();

	void LoadGame(const char *file);
	void SaveGame(const char *file);

	// Engine mode related...
	ENGINE_MODE GetEngineMode() const;
	void ChangeEngineMode();
	void PausePlayMode();

	// Debug Mode
	DebugMode debug_mode;
		
private:

	ENGINE_MODE engine_mode;

	bool	want_to_load = false;
	bool	want_to_save = false;
	char	load_game[SHORT_STRING];
	char	save_game[SHORT_STRING];

	char	app_name[SHORT_STRING];
	char	organization[SHORT_STRING];

	bool					app_marked_for_closing = false;
	std::list<Module*>		list_modules;
	
	void PrepareUpdate();
	void FinishUpdate();	

	void AddModule(Module* mod);

	void LoadConfig(const char *file_config, JSONParser &config);
	bool SaveGameNow();
	bool LoadGameNow();
};

extern Application *App;

#endif //!__APPLICATION_H__