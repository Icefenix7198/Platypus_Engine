#include "Performance.h"

Performance::Performance()
{
	real_time_timer.Start();
	game_time_timer.Start();

	real_time_delta_time = 0;
	game_time_delta_time = 0;
	game_time_scale = 1;

	memset(frame_rate_hist, 0, HISTOGRAM_VALUES);
	memset(miliseconds_hist, 0, HISTOGRAM_VALUES);

	frame_rate = 0;
	frames_on_last_update = 0;
	frame_count = 0;

	last_real_time = real_time_timer.Read();
	last_game_time = game_time_timer.Read();
	last_frames = 0;
	hist_last_update = 0;

	game_running = false;
	game_paused = false;
}

void Performance::PreUpdate()
{
	Uint32 current_time = real_time_timer.Read();
	real_time_delta_time = current_time - last_real_time;
	last_real_time = current_time;

	// When game is running
	if (game_running)
	{
		current_time = game_time_timer.Read();
		game_time_delta_time = current_time - last_game_time;
		last_game_time = current_time;
	}
}

void Performance::PostUpdate()
{
	// Delay to achieve cap framerate
	if (frame_rate != 0)
	{
		Uint32 delay = (Uint32)((1000.0f / frame_rate) - real_time_delta_time);
		if (delay > 0)
		{
			SDL_Delay(delay);
			real_time_delta_time += delay;
		}
	}

	// Amount of frames since startup
	frame_count++;

	Uint32 ms_since_startup = real_time_timer.Read();

	// Amount of frames during the last second (Update each 1/4 seconds)
	if (ms_since_startup - hist_last_update > 250)
	{
		frames_on_last_update = (float)(frame_count - last_frames) / (float)((ms_since_startup - hist_last_update) / 1000.0f);
		hist_last_update = ms_since_startup;
		last_frames = frame_count;
	}

	for (int i = 0; i < HISTOGRAM_VALUES - 1; ++i)
	{
		frame_rate_hist[i] = frame_rate_hist[i + 1];
		miliseconds_hist[i] = miliseconds_hist[i + 1];
	}

	frame_rate_hist[HISTOGRAM_VALUES - 1] = frames_on_last_update;
	miliseconds_hist[HISTOGRAM_VALUES - 1] = real_time_delta_time;
}

// Getters
Uint32 Performance::getMsDt()
{
	float dt;
	if (game_running)
	{
		if (game_paused)
			dt = 0;
		else
			dt = game_time_delta_time * game_time_scale;
	}
	else
		dt = real_time_delta_time;
	return dt;
}

float Performance::getSecDt()
{
	return getMsDt() / 1000.0f;
}

int &Performance::getFrameRate() { return frame_rate; }
float *Performance::getFrameRateHist() { return frame_rate_hist; }
float *Performance::getMilisecondsHist() { return miliseconds_hist; }

// Start or Stop game mode
void Performance::GameStart()
{
	game_time_timer.Start();
	game_running = true;
}

void Performance::GameStop()
{
	game_running = false;
	game_paused = false;
	last_game_time = 0;
}

// Pause or unpause game
bool Performance::IsGamePaused() const
{
	return game_paused;
}

void Performance::PauseGame()
{
	game_paused = true;
}

void Performance::UnpauseGame()
{
	game_paused = false;
}


