#ifndef _WIN_D_YESNO_
#define _WIN_D_YESNO_

#include <_type/type.dialog.h>
#include <_gadget/gadget.button.h>
#include <_gadget/gadget.label.h>
#include <_gadget/gadget.window.dialog.h>

class _yesNoDialog : public _dialog
{
	private:
		
		_button*		yesButton;
		_button*		noButton;
		_label*			msg;
		_dialogWindow*	window;
		
		_callbackReturn eventHandler( _event );
		
		void executeInternal();
		void cleanupInternal();
	
	public:
		
		//! Ctor
		//! @note if 'ignore'/nothing is passed as argument, the appropriate localized string is inserted instead
		_yesNoDialog( wstring message , wstring windowLabel , _optValue<wstring> yesLbl = ignore , _optValue<wstring> noLbl = ignore );
		
		//! Dtor
		~_yesNoDialog(){
			delete this->noButton;
			delete this->yesButton;
			delete this->msg;
			delete this->window;
		}
};

#endif