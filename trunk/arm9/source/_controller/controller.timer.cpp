#include <_controller/controller.timer.h>

#include <algorithm>

extern "C"{
	#include <nds/timers.h>
}

void _timerController::addTimerToExecute( _timer* timerToExecute ){
	_timerController::timersToExecute.push_back( timerToExecute );
}

bool _timerController::init()
{
	//! Start Timers
	TIMER_CR(0) = 0;
	TIMER_CR(1) = 0;
	TIMER_CR(2) = 0;
	
	TIMER_DATA(0) = 0;
	TIMER_DATA(1) = 0;
	TIMER_DATA(2) = 0;
	
	TIMER_CR(2) = TIMER_CASCADE | TIMER_ENABLE;
	TIMER_CR(1) = TIMER_CASCADE | TIMER_ENABLE;
	TIMER_CR(0) = TIMER_ENABLE;
	
	return true;
}

_tempTime _timerController::getMicroTime(){
	_tempTime time = _timerController::getRawTime();
	return
		( time >> 5 )
		- ( time >> 10 )
		- ( time >> 12 )
		- ( time >> 13 )
		- ( time >> 14 )
		- ( time >> 17 )
		- ( time >> 22 )
		- ( time >> 23 )
		- ( time >> 24 ); // Equals time/1000000
	/*
		BUS_CLOCK >> 5 = 1047311
		- (BUS_CLOCK>>10) = 1014583
		- (BUS_CLOCK>>12) = 1006401
		- (BUS_CLOCK>>13) = 1002310
		- (BUS_CLOCK>>14) = 1000256
		- (BUS_CLOCK>>17) = 1000011
		- (BUS_CLOCK>>22) = 1000004
		- (BUS_CLOCK>>23) = 1000001
		- (BUS_CLOCK>>24) = 1000000
		1000000 microseconds = 1 second = 33513982 Ticks
	*/
}

_tempTime _timerController::getMilliTime(){
	_tempTime time = _timerController::getRawTime();
	return ( time >> 15 ) - ( time >> 21 ) - ( time >> 22 ); // Equals time/1000
}

//! the speed in which the timer ticks in hertz.
//! #define BUS_CLOCK (33513982)
//! BUS_CLOCK >> 15 - BUS_CLOCK >> 21 - BUS_CLOCK >> 22 = 1000 * rawTime
//! There you get milliseconds from rawTime!

_tempTime _timerController::getRawTime()
{
	// Check Timers
	_u64 lo = TIMER_DATA(0);
	_u64 mi = TIMER_DATA(1);
	_u64 hi = TIMER_DATA(2);
	
	_u64 lo2 = TIMER_DATA(0);
	_u64 mi2 = TIMER_DATA(1);
	_u64 hi2 = TIMER_DATA(2);

	if( lo2 < lo )
		return ( lo2 | mi2<<16 | hi2 << 32);
	return ( lo | mi<<16 | hi << 32);
}

void _timerController::frame()
{
	_tempTime curTime = _timerController::getMilliTime();
	
	// Move runningTimers to execute
	move( _timerController::timersToExecute.begin() , _timerController::timersToExecute.end() , std::back_inserter( _timerController::runningTimers ) );
	
	// Clear buffer
	timersToExecute.clear();
	
	// Run all runningTimers
	_timerController::runningTimers.erase(
		std::remove_if(
			_timerController::runningTimers.begin()
			, _timerController::runningTimers.end()
			, [curTime]( _timer* timer )->bool
			{
				if( !timer || !timer->runs )
					return true;
				
				while( curTime > timer->duration + timer->startTime )
				{
					// Call...
					if( timer->callback )
						(*timer->callback)();
					
					if( timer->repeating )
						timer->startTime += timer->duration;
					else
					{
						timer->runs = false;
						return true;
					}
				}
				return false;
			}
		)
		, _timerController::runningTimers.end()
	);
}

_timerList	_timerController::runningTimers;
_timerList	_timerController::timersToExecute;