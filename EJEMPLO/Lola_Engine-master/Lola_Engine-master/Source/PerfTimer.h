#ifndef __PERFTIMER_H__
#define __PERFTIMER_H__

#include "Globals.h"

class PerfTimer
{
public:

	// Constructor
	PerfTimer();

	void Start();
	float ReadMs() const;
	uint64 ReadTicks() const;

private:
	uint64	started_at;
	static uint64 frequency;
};

#endif //__PerfTimer_H__
