// ----------------------------------------------------
// PerfTimer.cpp
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "PerfTimer.h"
#include "SDL\include\SDL_timer.h"

uint64 PerfTimer::frequency = 0;

// ---------------------------------------------
PerfTimer::PerfTimer()
{
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();
	Start();
}

// ---------------------------------------------
void PerfTimer::Start()
{
	started_at = SDL_GetPerformanceCounter();
}

// ---------------------------------------------
float PerfTimer::ReadMs() const
{
	return ((float)ReadTicks() / (float)frequency) * 1000.0f;
}

// ---------------------------------------------
uint64 PerfTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - started_at;
}


