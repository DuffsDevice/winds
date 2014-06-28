#ifndef _WIN_PROG_EXPLORER_
#define _WIN_PROG_EXPLORER_

#include "_type/type.program.c.h"
#include "_type/type.direntry.h"
#include "_gadget/gadget.fileView.h"
#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.button.image.h"
#include "_gadget/gadget.button.action.h"
#include "_gadget/gadget.window.menu.h"
#include "_gadget/gadget.window.bar.h"

class PROG_Explorer : public _progC
{
	private:
		
		string			path;
		
		_fileView*		fileView;
		_textBox*		addressBar;
		_button*		submitButton;
		_imageButton*	folderUpButton;
		_windowMenu*	windowMenu;
		_windowBar*		windowBar;
		
		void	main( _programArgs args );
		void	cleanUp();
		
		void setWindowTitle();
		
		_callbackReturn handler( _event event );
		
	public:
		
		PROG_Explorer();
};

#endif