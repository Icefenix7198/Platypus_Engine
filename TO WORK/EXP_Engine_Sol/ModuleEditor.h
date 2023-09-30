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
};

#endif //MODULE_EDITOR