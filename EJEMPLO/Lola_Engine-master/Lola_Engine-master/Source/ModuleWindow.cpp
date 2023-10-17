#include "ModuleWindow.h"

#include "Globals.h"
#include "Application.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl_gl3.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "Window");
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{ }

// Called before render is available
bool ModuleWindow::Awake(JSONParser &config)
{
	DEBUG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		DEBUG("[error] SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		screen_size = config.GetInt("screen_size");
		screen_height = config.GetInt("screen_height") * screen_size;
		screen_width = config.GetInt("screen_width") * screen_size;
		flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
		
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1); // 2 before change it

		if(config.GetBoolean("fullscreen")) flags |= SDL_WINDOW_FULLSCREEN;
		if(config.GetBoolean("resizable")) flags |= SDL_WINDOW_RESIZABLE;
		if(config.GetBoolean("borderless")) flags |= SDL_WINDOW_BORDERLESS;
		if(config.GetBoolean("fullscreen_desktop")) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		SDL_DisplayMode current;
		SDL_GetCurrentDisplayMode(0, &current);
		window = SDL_CreateWindow(App->GetAppName(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, flags);

		if(window == NULL)
		{
			DEBUG("[error] Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	DEBUG("Destroying SDL window and quitting all SDL systems");

	//Destroying surface
	if(screen_surface != NULL)
		SDL_FreeSurface(screen_surface);

	//Destroy window
	if(window != NULL)
		SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

bool ModuleWindow::Save(JSONParser &module)
{
	module.AddBoolean("Fullscreen", true);
	module.AddBoolean("V-Sync", false);

	return true;
}

bool ModuleWindow::Load(JSONParser &module)
{
	module.GetBoolean("Fullscreen");
	module.GetBoolean("V-Sync");
	return true;
}

uint ModuleWindow::GetScreenSize() const { return screen_size; }
int &ModuleWindow::GetScreenWidth() { return screen_width; }
int &ModuleWindow::GetScreenHeight() { return screen_height; }

void ModuleWindow::ChangeWindowSize()
{
	SDL_SetWindowSize(window, screen_width, screen_height);
}

void ModuleWindow::SetFullscreenOptions()
{
	if (flags & SDL_WINDOW_FULLSCREEN)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) 
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else 
		SDL_SetWindowFullscreen(window, 0);
}
