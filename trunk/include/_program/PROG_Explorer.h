#ifndef _WIN_PROG_EXPLORER_
#define _WIN_PROG_EXPLORER_

#include "_type/type.progC.h"
#include "_type/type.direntry.h"
#include "_gadget/gadget.fileview.h"
#include "_gadget/gadget.window.h"
#include "_gadget/gadget.textbox.h"

class PROG_Explorer : public _progC {
	
	private:
		
		string		path;
		
		_window*	window;
		_fileview*	fileview;
		_textbox*	addressbar;
		_button*	submitbutton;
		
		void		init( _cmdArgs );
		int			main( _cmdArgs );
		
		static _gadgetEventReturnType handler( _gadgetEvent event );
		
	public:
		
		PROG_Explorer( string path = "/" );
};

#endif