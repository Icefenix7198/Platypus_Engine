#ifndef __PERFORMANCE_H__
#define __PERFORMANCE_H__

#include "Timer.h"

//#include "Application.h"

#define HISTOGRAM_VALUES 60

// Variables here are used to separate Editor and Game behaviour

class Performance
{
private:

	// Arrays for ImGui graphic performance info
	float frame_rate_hist[HISTOGRAM_VALUES];
	float miliseconds_hist[HISTOGRAM_VALUES];

	// Timers and DT
	Timer real_time_timer;
	Timer game_time_timer;
	Uint32 real_time_delta_time;
	Uint32 game_time_delta_time;
	float game_time_scale;	

	int		frame_rate;		// Current framerate
	Uint32  frame_count;
	Uint32	frames_on_last_update;
	Uint32	last_real_time;
	Uint32	last_game_time;
	Uint32	hist_last_update;
	Uint32	last_frames;

	bool game_running;
	bool game_paused;

public:

	Performance();

	void PreUpdate();
	void PostUpdate();

	// Getters
	Uint32 getMsDt();
	float getSecDt();

	int &getFrameRate();
	float *getFrameRateHist();
	float *getMilisecondsHist();

	// Start or Stop game mode
	void GameStart();
	void GameStop();

	// Pause or unpause game
	bool IsGamePaused() const;
	void PauseGame();
	void UnpauseGame();
};

#endif //!__PERFORMANCE_H__

