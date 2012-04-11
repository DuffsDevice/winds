#ifndef _WIN_PROG_EXPLORER_
#define _WIN_PROG_EXPLORER_

#include "_type/type.progC.h"
#include "_type/type.directory.h"
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
		
		_directory*	dir;
		
		int			init( _cmdArgs );
		int			main( _cmdArgs );
		
	public:
		
		PROG_Explorer( string path = "/" );
};

#endif