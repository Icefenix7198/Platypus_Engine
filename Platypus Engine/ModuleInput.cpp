#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"

#include "ImGui/backends/imgui_impl_sdl2.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= App->window->GetScreenSize();
	mouse_y /= App->window->GetScreenSize();
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / App->window->GetScreenSize();
			mouse_y = e.motion.y / App->window->GetScreenSize();

			mouse_x_motion = e.motion.xrel / App->window->GetScreenSize();
			mouse_y_motion = e.motion.yrel / App->window->GetScreenSize();
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
				}
				break;
			}

			case SDL_DROPFILE:
			{
				
				std::string pathFile;
				std::string fileType;
				pathFile.assign(e.drop.file);
				LOG("Document with name %s droped", pathFile.c_str())
				bool pointFound=false;
				//We search were the point is in the string to select the fileType
				for (int i = 0; i < pathFile.size(); i++)
				{
					if(pointFound)
					{
						//Pushback the chars after the point (they will be the file tipe) ex: .fbx, .png, etc.
						fileType.push_back(pathFile.at(i));
					}
					if(pathFile.at(i) == '.')
					{
						pointFound = true; 
					}
				}

				if(std::strcmp(fileType.c_str(),"FBX")==0)
				{
					aasimp::Load(pathFile.c_str());
					LOG("Fbx File %s loaded", pathFile.c_str())
				}
				if (std::strcmp(fileType.c_str(), "fbx") == 0)
				{
					aasimp::Load(pathFile.c_str());
					LOG("Fbx File %s loaded", pathFile.c_str())
				}
				if (std::strcmp(fileType.c_str(), "png") == 0)
				{
					aasimp::LoadTexture(pathFile.c_str());
					LOG("Png File %s loaded", pathFile.c_str())
				}

				
				break;
			}
		}
	}

	if (quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
	{
		return UPDATE_STOP;
	}

	if (App->editor->quit == true)
	{
		return UPDATE_STOP;
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}