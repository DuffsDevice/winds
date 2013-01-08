// Check if already included
#ifndef _WIN_G_WINDOW_
#define _WIN_G_WINDOW_

#include "_type/type.gadget.h"
#include "_type/type.bitmap.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.button.h"
#include "_type/type.runtimeAttributes.h"

class _windowButton : public _button{
	private:
		_u8 buttonType;
		static _callbackReturn refreshHandler( _event event );
	public:
		_windowButton( _coord x , _coord y , _u8 buttonType );
};

class _window : public _gadget {
	
	private:
		
		_label* label;
		
		bool dragMe;
		
		_windowButton* button[3];
		
		static _callbackReturn refreshHandler( _event event );
		static _callbackReturn dragHandler( _event event );
		_callbackReturn buttonHandler( _event event ); // Handler for _window-Buttons
		static _callbackReturn restyleHandler( _event event );
		
		// Will be called if the window is resized ->label will also be resized
		static _callbackReturn resizeHandler( _event e );
		
	public:
		
		void setStrValue( string title ){ this->label->setStrValue( title ); }
		
		string getStrValue(){ return this->label->getStrValue(); }
		
		//! Ctor
		_window( _length width , _length height , _coord x , _coord y , string title , _style style = _style() );
		
		//! Dtor
		~_window();
};

#endif