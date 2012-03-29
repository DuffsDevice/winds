// Check if already included
#ifndef _WIN_WINDOWS_
#define _WIN_WINDOWS_

//! Types
#include "_type/type.gadget.h"
#include "_type/type.gadgetStyle.h"
#include "_type/type.runtimeAttributes.h"

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
		
		//! Events
		deque< _gadgetEvent > events;
		deque< _gadgetEvent > newEvents;
		bool eventThrowable;
		
		//! Event-Reducer
		void optimizeEvents();
		
		//! Event-Handling
		void enableEventThrowing( void );
		
		
		//! Event-Not-Handling
		void disableEventThrowing( void );
		
		//! Standard Function to be executed on refresh
		static _gadgetEventReturnType	refreshHandler( _gadgetEvent );
		
		bool focusChild( _gadget* child );
		
	public:
		
		//! Runtime Attributes
		_runtimeAttributes runtimeAttributes;
		
		//! Standard VBL is provided
		void VBlank();
		
		// Obtain current Keys
		_u16 getCurrentKeys();
		
		//! Process User Inputs
		void processInput();
		
		
		//! Add an Event to the stack
		//! See Gadget: Line 113
		void generateEvent( _gadgetEvent event , bool works = true );
		
		
		//! Process the Events
		void processEvents();
		
		
		//! Constructor with style
		_windows( _gadgetStyle style = _defaultStyle_ );
		
		
		//! Get Taskboard of windows
		_windowsTaskBoard* getTaskBoard(){ return this->taskboard; }
		
};


/*// Miscelanious
namespace DSWindows {
	
	void convertBMP( _pixel *data , u32 size )
	{
		for( u32 f = 0 ; f <= size ; ++f )
		{
			// Circular right Shift
			data[f] = ROR16( data[f] , 1 );
			
			
			// Reordering
			data[f] = RGB( RGB_GETB( data[f] ) , // Because the Values are ordered in other direction
			                 RGB_GETG( data[f] ) , RGB_GETR( data[f] ) );
		}
	}
	
	void registerWindow( _gadget* window ){
		Windows.addChild( window );
	}
}*/

#endif