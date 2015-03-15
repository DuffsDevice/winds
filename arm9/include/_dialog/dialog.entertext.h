#ifndef _WIN_D_ENTERTEXT_
#define _WIN_D_ENTERTEXT_

#include <_type/type.dialog.h>
#include <_gadget/gadget.button.h>
#include <_gadget/gadget.label.h>
#include <_gadget/gadget.window.dialog.h>
#include <_gadget/gadget.textbox.h>

class _enterTextDialog : public _dialog
{
	private:
		
		_button*		okButton;
		_button*		cancelButton;
		_label*			msg;
		_textBox*		textBox;
		_dialogWindow*	window;
		wstring			initialValue;
		
		_callbackReturn eventHandler( _event );
		
		void executeInternal();
		void cleanupInternal();
	
	public:
		
		//! Ctor
		//! @note if 'ignore'/nothing is passed as argument, the appropriate localized string is inserted instead
		_enterTextDialog( wstring message , wstring windowLabel , wstring initialValue = "" , _optValue<wstring> okLabel = ignore , _optValue<wstring> cancelLabel = ignore );
		
		//! Get the Entered string
		wstring getResult(){
			return this->textBox->getStrValue();
		}
		
		//! Set value to be displayed on default
		void setInitialValue( wstring initialValue ){
			this->initialValue = move(initialValue);
		}
		
		//! Get initial Value
		wstring getInitialValue(){
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