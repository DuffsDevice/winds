#include "_type/type.timer.h"
#include "_type/type.system.h"

_timerList _timer::globalTimers;
_timerList _timer::globalTimersToExecute;

void _timer::start()
{
	this->startTime = _system::getHighResTime();
	this->runs = true;
	
	_timer::globalTimersToExecute.push_back( this ); // kotzen
}

void _timer::stop()
{
	if( !this->runs )
		return;
	
	this->runs = false;
}

_timer& _timer::operator=( _timer&& tmr )
{
	this->deleteCallback();
	
	this->duration = tmr.duration;
	this->repeating = tmr.repeating;
	this->runs = tmr.runs;
	this->callback = tmr.callback;
	
	tmr.callback = nullptr;
	
	// Rewrite timer in list
	if( tmr.runs ){
		_timerList::iterator it = find( _timer::globalTimers.begin() , _timer::globalTimers.end() , &tmr );
		if( it != _timer::globalTimers.end() )
			(*it) = this;
	}
	
	return *this;
}

void _timer::runTimers()
{
	_tempTime curTime = _system::getHighResTime();
	
	// Move timers to execute
	move( _timer::globalTimersToExecute.begin() , _timer::globalTimersToExecute.end() , std::back_inserter( _timer::globalTimers ) );
	
	// Clear buffer
	globalTimersToExecute.clear();
	
	// Run all timers
	_timer::globalTimers.erase(
		std::remove_if(
			_timer::globalTimers.begin()
			, _timer::globalTimers.end()
			, [curTime]( _timer* timer )->bool
			{
				if( !timer->runs )
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
		, _timer::globalTimers.end()
	);
}