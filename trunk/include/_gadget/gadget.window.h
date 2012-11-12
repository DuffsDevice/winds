// Check if already included
#ifndef _WIN_G_WINDOW_
#define _WIN_G_WINDOW_

#include "_type/type.gadget.h"
#include "_type/type.bitmap.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.button.h"
#include "interface.input.h"
#include "_type/type.runtimeAttributes.h"

class _windowButton : public _button{
	private:
		_u8 buttonType;
		static _callbackReturn refreshHandler( _event event );
	public:
		_windowButton( _coord x , _coord y , _u8 buttonType );
};

class _window : public _gadget , public _interface_input {
	
	private:
		
		_label* label;
		
		bool dragMe;
		
		_windowButton* button[3];
		
		static _callbackReturn refreshHandler( _event event );
		
		static _callbackReturn dragHandler( _event event );
		
		static _callbackReturn closeHandler( _event event );
		
		// Will be called if the window is resized ->label will also be resized
		static _callbackReturn resizeHandler( _event e );
		
	public:
		
		void setTitle( string title ){ if( this->label ) this->label->setStrValue( title ); }
		
		string getTitle(){ if( this->label ) return this->label->getStrValue(); return ""; }
		
		_window( _length width , _length height , _coord x , _coord y , string title , _style style = _style());
};

#endif