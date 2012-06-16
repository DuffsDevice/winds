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

#include "_gadget/gadget.keyboard.h"
#include "_gadget/gadget.desktop.h"

class _windows : public _gadget
{
	
	private:
	
		//! Background-id
		_u8					bgId;
		
		//! Graphics
		BMP_WindowHeader	winHeader;
		
		//! Desktop-Gadget
		_desktop*			desktop;
		
		//! Standard Function to be executed on refresh
		static _gadgetEventReturnType	refreshHandler( _gadgetEvent );
		
		bool focusChild( _gadget* child );
		
	public:		
	
		//! Constructor with style
		_windows( _u8 bgId , _gadgetStyle style = _defaultStyle_ );
		
};

#endif