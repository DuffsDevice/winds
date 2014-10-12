#ifndef _WIN_C_TIMER_
#define _WIN_C_TIMER_

#include <_type/type.h>
#include <_type/type.controller.h>
#include <_type/type.timer.h>

typedef _vector<_timer*> _timerList;

class _timerController : public _controller
{
	private:
		
		static _timerList	runningTimers;
		static _timerList	timersToExecute;
		static bool			currentlyIterating;
		
		friend class _timer;
		
		static void registerTimerToExecute( _timer* timerToExecute );
		static void deleteTimer( _timer* timerToDelete );
	
	public:
		
		//! Inits the interrupt system
		static bool init();
		
		//! Runs all timers
		static void frame();
		
		//! Get time since system startup
		static _tempTime getMilliTime();	//! in Milliseconds
		static _tempTime getMicroTime();	//! in Microseconds since system startup
		static _tempTime getRawTime();	//! Clock Ticks at 33.513982 MHz
};

#endif