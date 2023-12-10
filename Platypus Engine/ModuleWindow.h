#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	//Configuration getters and setters
	int GetScreenWidth() { return SCREEN_WIDTH; }
	int GetScreenHeigth() {return SCREEN_HEIGHT;}
	float GetScreenSize() { return SCREEN_SIZE; }
	bool GetWinFullscreen() { return WIN_FULLSCREEN; }
	bool GetWinResizable() {return WIN_RESIZABLE; }
	bool GetWinFullscreenDesktop() {return WIN_FULLSCREEN_DESKTOP; }
	bool GetWinBorderless() { return WIN_BORDERLESS; }
	bool GetVSYNC() { return VSYNC; }

	void SetScreenWidth(int value) { SCREEN_WIDTH = value; }
	void SetScreenHeigth(int value) { SCREEN_HEIGHT = value; }
	void SetScreenSize(float value) { SCREEN_SIZE = value;}
	void SetWinFullscreen(bool value) 
	{ 
		WIN_FULLSCREEN = value;
		WIN_FULLSCREEN_DESKTOP = false;
	}
	void SetWinResizable(bool value){ WIN_RESIZABLE = value; }
	void SetWinFullscreenDesktop(bool value) 
	{ 
		WIN_FULLSCREEN_DESKTOP = value;
		WIN_FULLSCREEN = false;
	}
	void SetWinBorderless(bool value) { WIN_BORDERLESS = value; }
	void SetVSYNC(bool value){ VSYNC = value; }

	void ResizeWindow(); 


private:
	// Configuration -----------
	int SCREEN_WIDTH = 1280;
	int SCREEN_HEIGHT = 1024;
	float SCREEN_SIZE = 0.7; //1 originalmente pero se nos salia de la pantalla
	bool WIN_FULLSCREEN = false;
	bool WIN_RESIZABLE = true;
	bool WIN_BORDERLESS = false;
	bool WIN_FULLSCREEN_DESKTOP = false;
	bool VSYNC = true; //Quiza hay que cambiar esto a bool
	#define TITLE "Platypus Engine"
};

#endif // __ModuleWindow_H__