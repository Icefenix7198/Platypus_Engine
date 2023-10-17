#pragma once
#ifndef MODULE_SCENE
#define MODULE_SCENE

#include "Module.h"
#include "Globals.h"
#include <vector>
#include <string>

class ModuleScene : public Module
{
public:

	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init() override;
	bool CleanUp() override;

	//Quiza esto sea too much y haya que descartar la idea
private:
	
};

#endif //MODULE_SCENE