#pragma once
#ifndef MODULE_EDITOR
#define MODUEL_EDITOR

#include "Module.h"
#include "Globals.h"
#include <vector>

class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init() override;
	void DrawEditor();
	bool CleanUp() override;

	void AddFPS(const float aFPS);

	std::vector<float> mFPSLog;

	int cpuCache;
	int cpus;
	int ram;

	SDL_bool caps[11];

	//Quiza esto sea too much y haya que descartar la idea
private:
	bool showFPS;
};

#endif //MODULE_EDITOR