// Check if already included
#ifndef _WIN_WINDOWS_
#define _WIN_WINDOWS_

//! Types
#include "_type/type.gadget.h"
#include "_type/type.gadgetScreen.h"

#include "_gadget/gadget.keyboard.h"
#include "_gadget/gadget.desktop.h"
#include "_gadget/gadget.windows.button.h"

class _windows : public _gadgetScreen
{
	
	private:
		
		//! Start-Button
		_windowsStartButton*	startButton;
		
		//! Desktop-Gadget
		_desktop*				desktop;
		
		//! Standard Function to be executed on refresh
		static _callbackReturn	refreshHandler( _event );
		
	public:		
	
		//! Constructor with style
		_windows( _u8 bgId , _style style = _style() );
		
};

#endif