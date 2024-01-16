#pragma once
#ifndef MODULE_EDITOR
#define MODUEL_EDITOR

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "imGui/imgui.h"
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

	void AddDeltaTime(const float aFPS);
	std::vector<float> mDTLog;

	void AddFPS(const float aFPS);
	std::vector<float> mFPSLog;

	void ConsoleLog(const std::string& str);

	void GameObjectHierarchy(GameObject* go);

	void Inspector(GameObject* go);

	void AssetsMenu();

	float GetPlayDT() { return dtsaved; }
	bool GetPaused() { return ispaused; }
	void SetPlayDT(bool pause) { ispaused = pause; }

	int cpuCache;
	int cpus;
	int ram;

	SDL_bool caps[11];

	bool quit = false;

	SDL_version SDLversion;

	//Quiza esto sea too much y haya que descartar la idea
private:
	bool showFPS;
	bool showConsole = true;
	bool showConfig = true;
	bool showDemo = false;
	bool showInspector = true; //No se si inspector es la palabra correcta
	bool assetsWindow = true;

	bool isplaying = false;
	bool ispaused = false;

	float dtsaved = 0.0f;

	//For generic position in screen
	const ImGuiViewport* main_viewport;
};

#endif //MODULE_EDITOR