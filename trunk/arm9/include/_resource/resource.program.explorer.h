#ifndef _WIN_PROG_EXPLORER_
#define _WIN_PROG_EXPLORER_

#include "_type/type.program.c.h"
#include "_type/type.direntry.h"
#include "_gadget/gadget.fileView.h"
#include "_gadget/gadget.window.h"
#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.button.image.h"

class PROG_Explorer : public _progC
{
	private:
		
		string			path;
		
		_window*		window;
		_fileView*		fileView;
		_textBox*		addressBar;
		_button*		submitButton;
		_imageButton*	folderUpButton;
		
		void		main( _programArgs args );
		void		destruct();
		
		void setWindowTitle();
		
		_callbackReturn handler( _event event );
		
	public:
		
		PROG_Explorer();
};

#endif