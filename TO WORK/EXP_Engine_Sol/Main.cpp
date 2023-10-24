#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

int main(int argc, char ** argv)
{
	LOG("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* App = NULL;

	uint startTime;
	float fps;

	startTime = SDL_GetTicks();

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			LOG("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			LOG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}

		switch (App->timeControl)
		{

		case DeltaTimeControl::FIXED_DELTATIME:

			App->dt = (1/App->fixedFPS);

			break;
		case DeltaTimeControl::FIXED_DELTATIME_DELAY:

			App->dt = (1 / App->fixedFPS);

			if ((1 / App->fixedFPS) > SDL_GetTicks() - startTime)
			{
				SDL_Delay((1 / App->fixedFPS) - (SDL_GetTicks() - startTime));
			}

			break;
		case DeltaTimeControl::VARIABLE_DELTATIME:

			if ((1 / App->fixedFPS) > SDL_GetTicks() - startTime)
			{
				SDL_Delay(App->fixedFPS - (SDL_GetTicks() - startTime));
			}
			fps = (SDL_GetTicks() - startTime);
			App->dt = fps;

			break;
		default:
			break;
		}

	}

	delete App;
	LOG("Exiting game '%s'...\n", TITLE);
	return main_return;
}