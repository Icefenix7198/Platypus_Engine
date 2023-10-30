#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"

#include<vector>

enum class DeltaTimeControl
{

	FIXED_DELTATIME,
	FIXED_DELTATIME_DELAY,
	VARIABLE_DELTATIME

};

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ModuleScene* scene;

	//Moved to public to control Delta Time
	Timer	ms_timer;
	float fixedFPS = 60; //16 es too fast 
	float	dt;
	DeltaTimeControl timeControl = DeltaTimeControl::FIXED_DELTATIME;

private:

	
	std::vector<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	float GetDeltaTime();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;

#endif //!__APPLICATION_H__