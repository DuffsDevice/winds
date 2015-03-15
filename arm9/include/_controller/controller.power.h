#ifndef _WIN_C_POWER_
#define _WIN_C_POWER_

#include <_type/type.h>
#include <_type/type.controller.h>
#include <_type/type.uniqueptr.h>
#include <_type/type.callback.h>
#include <_type/type.paramalloc.h>

class _powerController : public _controller
{
	private:
		
		static _vector<_uniquePtr<_callback<void(void)>>> beforeShutdownHandlers;
		
	public:
		
		//! Inits the power controller
		static bool init();
		
		//! Terminates the power controller and shuts down the system
		static void end();
		
		//! Shuts down the system
		static void shutDown();
		
		//! Causes the DS to go into sleep mode. It will be reawaked when the Lid openes
		static void sleep();
		
		//! Get the DS' Battery Level
		static int getBatteryLevel();
		
		//! Method to add functions that get called
		static void addShutdownCallback( _paramAlloc<_callback<void(void)>> cb ){
			beforeShutdownHandlers.push_back( cb.get() );
		}
};

#endif