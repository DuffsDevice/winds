#ifndef _WIN_G_SETUP_
#define _WIN_G_SETUP_

#include "_type/type.gui.h"
#include "_type/type.view.switcher.h"
#include "_gadget/gadget.button.action.h"
#include "_gadget/gadget.label.h"

class _guiSetup : public _gui
{
	private:
		
		_viewSwitcher	viewSwitcher;
		_actionButton*	nextButton;
		_actionButton*	prevButton;
		_label*			nextLabel;
		_label*			prevLabel;
		
		_callbackReturn	nextButtonClickHandler( _event event );
		_callbackReturn prevButtonClickHandler( _event event );
		_callbackReturn keyHandler( _event event );
		
		void placeButtonsAndLabels();
	
	public:
		
		//! Ctor
		_guiSetup();
		
		//! Dtor
		~_guiSetup();
};

#endif