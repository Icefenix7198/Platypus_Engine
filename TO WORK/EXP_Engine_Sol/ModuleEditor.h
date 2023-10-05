#pragma once
#ifndef MODULE_EDITOR
#define MODUEL_EDITOR

#include "Module.h"
#include "Globals.h"
#include <vector>
#include <string>

//inline std::string stringTest;

class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init() override;
	void DrawEditor();
	bool CleanUp() override;

	//ImGui Complicated Things Print
	void AddFPS(const float aFPS);
	

	std::vector<float> mFPSLog;
	//extern std::string consoleLog; //La classe de almacenamiento es no valida
	std::string consoleLog;
	
	void ConsoleLog(const std::string& str);

	int cpuCache;
	int cpus;
	int ram;

	SDL_bool caps[11];



	//Quiza esto sea too much y haya que descartar la idea
private:
	bool showFPS;
	bool showConsole = true;
	bool showConfig;
	bool showDemo = false;
};

#endif //MODULE_EDITOR