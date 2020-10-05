#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "SDL\include\SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();
	int delay=4000;
	bool d = false;

	Uint32 Read();

private:

	bool	running;
	Uint32	started_at;
	Uint32	stopped_at;
};

class j1PerfTimer
{
public:

	// Constructor
	j1PerfTimer();

	void Start();
	double ReadMs() const;
	double ReadTicks() const;

private:
	double	started_at = 0;
	static Uint64 frequency;
	
};


#endif //__TIMER_H__