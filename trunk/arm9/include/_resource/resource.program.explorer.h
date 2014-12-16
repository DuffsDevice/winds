#ifndef _WIN_PROG_EXPLORER_
#define _WIN_PROG_EXPLORER_

#include <_type/type.program.c.h>
#include <_type/type.direntry.h>
#include <_type/type.view.switcher.h>
#include <_gadget/gadget.fileView.h>
#include <_gadget/gadget.textbox.h>
#include <_gadget/gadget.button.image.h>
#include <_gadget/gadget.button.action.h>
#include <_gadget/gadget.window.menu.h>
#include <_gadget/gadget.window.bar.h>
#include <_gadget/gadget.scrollarea.h>


class PROG_Explorer : public _progC
{
	private:
		
		string			path;
		
		_fileView*		fileView;
		_scrollArea*	scrollArea;
		_textBox*		addressBar;
		_button*		submitButton;
		_imageButton*	folderUpButton;
		_windowMenu*	windowMenu;
		_windowBar*		windowBar;
		
		void	main( _args args );
		void	cleanUp();
		
		void	setWindowTitle();
		void	updateFileView();
		
		_callbackReturn handler( _event event );
		
		//! Object for managing special sites
		_viewSwitcher	viewSwitcher;
		
	public:
		
		//! Only used by special sites to change the path again
		void setNewPath( string path ){
			this->fileView->setPath( move(path) );
			this->fileView->triggerEvent( onEdit );
		}
		
		PROG_Explorer();
};

#endif