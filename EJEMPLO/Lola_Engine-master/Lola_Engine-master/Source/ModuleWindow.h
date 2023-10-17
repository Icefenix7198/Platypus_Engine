#ifndef __MODULEWINDOW_H__
#define __MODULEWINDOW_H__

#include "Module.h"
#include "SDL\include\SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Awake(JSONParser &config);
	bool CleanUp();

	void SetTitle(const char* title);
	uint GetScreenSize() const;
	int &GetScreenWidth();
	int &GetScreenHeight();

	void ChangeWindowSize();
	void SetFullscreenOptions();

public:
	//Window flags
	Uint32 flags;

	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:

	int screen_width;
	int screen_height;
	uint screen_size;

	bool Save(JSONParser &module);
	bool Load(JSONParser &module);
};

#endif // __MODULEWINDOW_H__