#ifndef _WIN_D_IMAGE_
#define _WIN_D_IMAGE_

#include <_type/type.dialog.h>
#include <_gadget/gadget.button.h>
#include <_gadget/gadget.label.h>
#include <_gadget/gadget.window.dialog.h>
#include <_gadget/gadget.image.h>

class _imageDialog : public _dialog
{
	protected:
		
		_button*		okButton;
		_button*		otherButton;
		_label*			msg;
		_dialogWindow*	window;
		_imageGadget*	image;
		
		_callbackReturn eventHandler( _event );
		
		virtual void executeInternal();
		void cleanupInternal();
	
	public:
		
		//! Ctor(s)
		//! @note if 'ignore'/nothing is passed as argument, the appropriate localized string is inserted instead
		_imageDialog( string message , string windowLabel , _bitmap&& image , _optValue<string> okLabel = ignore , _optValue<string> otherLabel = ignore );
		_imageDialog( string message , string windowLabel , _constBitmap& image , _optValue<string> okLabel = ignore , _optValue<string> otherLabel = ignore ) :
			_imageDialog( move(message) , move(windowLabel) , _bitmap(image) , move(okLabel) , move(otherLabel) )
		{}
		
		//! Dtor
		~_imageDialog(){
			delete this->okButton;
			delete this->otherButton;
			delete this->msg;
			delete this->window;
			delete this->image;
		}
};

#endif