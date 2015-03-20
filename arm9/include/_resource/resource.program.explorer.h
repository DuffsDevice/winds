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

class _explorerPageFolder;

class PROG_Explorer : public _cProgram , public _viewSwitcher
{
	private:
		
		_uniquePtr<_explorerPageFolder>	folderPage;
		_uniquePtr<_textBox>			addressBar;
		_uniquePtr<_button>				submitButton;
		_uniquePtr<_imageButton>		folderUpButton;
		_uniquePtr<_windowMenu>			windowMenu;
		_uniquePtr<_windowBar>			windowBar;
		
		// Routine called to execute the Explorer (will be called by _cProgram)
		void	main( _args args );
		
		// Eventhandler for arbitrary events
		_callbackReturn handler( _event event );
		
		//! Overrides _viewSwitcher-methods
		_view*	getViewByName( string assocName ) const override ;
		string	beforeChange( string newViewName ) const override ;
		
	public:
		
		//! Ctor
		PROG_Explorer();
};

#endif