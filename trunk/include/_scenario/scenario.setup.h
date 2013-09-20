#ifndef _WIN_SC_SETUP_
#define _WIN_SC_SETUP_

#include "_type/type.scenario.h"
#include "_type/type.viewswitcher.h"
#include "_gadget/gadget.actionbutton.h"
#include "_gadget/gadget.label.h"

class _scSetup : public _scenario
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
		_scSetup();
		
		//! Dtor
		~_scSetup();
};

#endif