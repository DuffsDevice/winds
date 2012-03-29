#ifndef _WIN_G_TASKBOARD_
#define _WIN_G_TASKBOARD_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include "_type/type.animation.h"
#include "_type/type.runtimeAttributes.h"

#include "_gadget/gadget.keyboard.h"

//! Screen_Height etc...
#include "nds/arm9/console.h"

class _windowsTaskBoard : public _gadget{
	
	private:
		
		// Taskbar Bg
		static _bitmap* startButton;
		
		_animation<int>*	anim;
		
		// Keyboard
		_keyboard*	keyboard;
		
		bool dragMe;
		
		//! Whether the Keyboard is collapsed(=0) or expanded(=1)
		bool		mode;
		
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
		
		static _gadgetEventReturnType dragHandler( _gadgetEvent event );
		
	public:
		
		//! Open the Keyboard and set the Handler of key-Events
		void openDialog( _gadget* dest , _gadget* visibleArea = nullptr );
		
		_windowsTaskBoard();
		
		~_windowsTaskBoard();
};

#endif