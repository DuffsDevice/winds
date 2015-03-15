#include <_type/type.timer.h>
#include <_controller/controller.timer.h>

void _timer::start()
{
	this->startTime = _timerController::getMilliTime();
	this->runs = true;
	
	_timerController::addTimerToExecute( this ); // Kotzen!
}

void _timer::stop(){
	this->runs = false;
}

_timer& _timer::operator=( _timer&& tmr )
{
	this->duration = tmr.duration;
	this->repeating = tmr.repeating;
	this->runs = tmr.runs;
	this->callback = move(tmr.callback);
	
	return *this;
}