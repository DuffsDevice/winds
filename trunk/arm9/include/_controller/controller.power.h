#ifndef _WIN_C_POWER_
#define _WIN_C_POWER_

#include "_type/type.h"
#include "_type/type.controller.h"

class _powerController : public _controller
{
	public:
		
		//! Inits the interrupt system
		static bool init();
		
		//! Shuts down the system
		static void shutDown();
		
		//! Causes the DS to go into sleep mode. It will be reawaked when the Lid openes
		static void sleep();
		
		//! Get the DS' Battery Level
		static int getBatteryLevel();
};

#endif