#ifndef __MOUDLESCENEINTRO_H__
#define __MODULESCENEINTRO_H__

#include "Module.h"
#include "Globals.h"

class ModuleSceneIntro : public Module
{

public:

	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	UPDATE_STATUS Update(float dt);
	bool CleanUp();

};

#endif //! __MODULESCENEINTRO_H__
