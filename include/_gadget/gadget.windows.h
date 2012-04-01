// Check if already included
#ifndef _WIN_WINDOWS_
#define _WIN_WINDOWS_

//! Types
#include "_type/type.gadget.h"

// C-Header
#include <stdio.h>
#include <list>
#include <algorithm>

// Graphics
#include "_graphic/BMP_WindowsWallpaper.h"
#include "_graphic/BMP_WindowHeader.h"
#include "_graphic/FONT_CourierNew10.h"
#include "_graphic/FONT_Tahoma7.h"

#include "_gadget/gadget.windows.taskBoard.h"
#include "_gadget/gadget.window.h"
#include "_gadget/gadget.desktop.h"

class _windows : public _gadget{
		
	private:
		
		//! Graphics
		BMP_WindowHeader 	 winHeader;
		
		//! Taskbar and Keyboard!
		_windowsTaskBoard*	taskboard;
		_desktop*			desktop;
		
		//! Standard Function to be executed on refresh
		static _gadgetEventReturnType	refreshHandler( _gadgetEvent );
		
		bool focusChild( _gadget* child );
		
	public:		
	
		//! Constructor with style
		_windows( _gadgetStyle style = _defaultStyle_ );
		
		
		//! Get Taskboard of windows
		_windowsTaskBoard* getTaskBoard(){ return this->taskboard; }
		
};

#endif