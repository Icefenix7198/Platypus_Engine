#pragma once
#ifndef MODULE_EDITOR
#define MODUEL_EDITOR

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
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
	void Configuration();

	void AddFPS(const float aFPS);
	std::vector<float> mFPSLog;

	void ConsoleLog(const std::string& str);

	void GameObjectHierarchy(GameObject* go);

	void Inspector(GameObject* go);

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
	bool showInspector = true; //No se si inspector es la palabra correcta
};

#endif //MODULE_EDITOR