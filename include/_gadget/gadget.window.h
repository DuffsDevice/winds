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
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
	public:
		_windowButton( _coord x , _coord y , _u8 buttonType );
};

class _window : public _gadget , public _interface_input {
	
	private:
		
		_label* label;
		
		bool dragMe;
		
		_windowButton* button[3];
		
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
		
		static _gadgetEventReturnType dragHandler( _gadgetEvent event );
		
		static _gadgetEventReturnType closeHandler( _gadgetEvent event );
		
		// Will be called if the window is resized ->label will also be resized
		static _gadgetEventReturnType resizeHandler( _gadgetEvent e );
		
	public:
		
		void setTitle( string title ){ if( this->label ) this->label->setStrValue( title ); }
		
		string getTitle(){ if( this->label ) return this->label->getStrValue(); return ""; }
		
		_window( _length width , _length height , _coord x , _coord y , string title , _gadgetStyle style = _defaultStyle_);
};

#endif