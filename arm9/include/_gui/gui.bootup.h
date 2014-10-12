#ifndef _WIN_G_BOOTUP_
#define _WIN_G_BOOTUP_

#include <_type/type.gui.h>
#include <_type/type.timer.h>

class _guiBootup : public _gui
{
	private:
		
		_timer timer;
		
		//! Callback for the above timer
		static void timerCallback();
		
	public:
		
		//! Ctor
		_guiBootup();
};

#endif