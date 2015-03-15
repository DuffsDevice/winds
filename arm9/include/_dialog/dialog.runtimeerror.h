#ifndef _WIN_D_RUNTIMEERROR_
#define _WIN_D_RUNTIMEERROR_

#include <_type/type.dialog.h>
#include <_gadget/gadget.button.h>
#include <_gadget/gadget.label.h>
#include <_gadget/gadget.window.dialog.h>
#include <_gadget/gadget.image.h>
#include <_gadget/gadget.textarea.h>

class _runtimeErrorDialog : public _dialog
{
	private:
		
		_button*		closeButton;
		_label*			msg;
		_dialogWindow*	window;
		_imageGadget*	image;
		_textArea*		additionalInfo;
		
		_callbackReturn eventHandler( _event );
		
		static _constBitmap errorimage;
		
		void executeInternal();
		void cleanupInternal();
	
	public:
		
		//! Ctor(s)
		//! @note if 'ignore'/nothing is passed as argument, the appropriate localized string is inserted instead
		_runtimeErrorDialog( wstring message , wstring additionalInfo = "" );
		
		//! Dtor
		~_runtimeErrorDialog(){
			delete this->closeButton;
			delete this->msg;
			delete this->window;
			delete this->image;
			delete this->additionalInfo;
		}
};

#endif