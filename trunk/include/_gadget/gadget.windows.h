// Check if already included
#ifndef _WIN_WINDOWS_
#define _WIN_WINDOWS_

//! Types
#include "_type/type.gadget.h"
#include "_type/type.gadgetScreen.h"

//! C-Header
#include <stdio.h>
#include <list>
#include <algorithm>

//! Graphics
#include "_resource/BMP_WindowsWallpaper.h"
#include "_resource/BMP_WindowHeader.h"
#include "_resource/FONT_CourierNew10.h"
#include "_resource/FONT_Tahoma7.h"

#include "_gadget/gadget.keyboard.h"
#include "_gadget/gadget.desktop.h"
#include "_gadget/gadget.windows.button.h"

class _windows : public _gadgetScreen
{
	
	private:
		
		//! Graphics
		BMP_WindowHeader		winHeader;
		
		//! Start-Button
		_windowsStartButton*	startButton;
		
		//! Desktop-Gadget
		_desktop*				desktop;
		
		//! Standard Function to be executed on refresh
		static _gadgetEventReturnType	refreshHandler( _gadgetEvent );
		
		//bool focusChild( _gadget* child );
		
	public:		
	
		//! Constructor with style
		_windows( _u8 bgId , _gadgetStyle style = _defaultStyle_ );
		
};

#endif