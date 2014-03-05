#ifndef _WIN_D_ENTERTEXT_
#define _WIN_D_ENTERTEXT_

#include "_type/type.dialog.h"
#include "_gadget/gadget.button.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.window.h"
#include "_gadget/gadget.textbox.h"

class _enterTextDialog : public _dialog
{
	private:
		
		_button*	okButton;
		_button*	cancelButton;
		_label*		msg;
		_textBox*	textBox;
		_window*	window;
		string		initialValue;
		
		_callbackReturn eventHandler( _event );
		
		void executeInternal();
		void cleanupInternal();
	
	public:
		
		//! Ctor
		//! @note if 'ignore'/nothing is passed as argument, the appropriate localized string is inserted instead
		_enterTextDialog( string message , string windowLabel , string initialValue = "" , _optValue<string> okLabel = ignore , _optValue<string> cancelLabel = ignore );
		
		//! Get the Entered string
		string getResult(){
			return this->textBox->getStrValue();
		}
		
		//! Set value to be displayed on default
		void setInitialValue( string initialValue ){
			this->initialValue = move(initialValue);
		}
		
		//! Get initial Value
		string getInitialValue(){
			return this->initialValue;
		}
		
		//! Dtor
		~_enterTextDialog(){
			delete this->okButton;
			delete this->cancelButton;
			delete this->msg;
			delete this->textBox;
			delete this->window;
		}
};

#endif