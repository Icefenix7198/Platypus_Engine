#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL\include\SDL.h"
#pragma comment( lib, "Source/SDL/libx86/SDL2.lib" )
#pragma comment( lib, "Source/SDL/libx86/SDL2main.lib" )

enum MAIN_STATES
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application *App = nullptr;

int main(int argc, char ** argv)
{
	int main_return = EXIT_FAILURE;
	MAIN_STATES state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			App = new Application();
			DEBUG("Starting Engine");
			DEBUG("-------------- Application Creation --------------");
			state = MAIN_START;
			break;

		case MAIN_START:

			DEBUG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				DEBUG("[error] Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				DEBUG("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				DEBUG("[error] Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			DEBUG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				DEBUG("[error] Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	DEBUG("Exiting Engine");
	delete App; //RELEASE(App);
	
	return main_return;
}