#ifndef _WIN_SC_BOOTUP_
#define _WIN_SC_BOOTUP_

#include "_type/type.scenario.h"
#include "_type/type.timer.h"

class _scBootup : public _scenario
{
	private:
		
		_timer timer;
		
	public:
		
		//! Ctor
		_scBootup();
};

#endif