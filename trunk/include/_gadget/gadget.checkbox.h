// Check if already included
#ifndef _WIN_G_CHECKBOX_
#define _WIN_G_CHECKBOX_

#include "_type/type.gadget.h"
#include "_gadget/gadget.label.h"
#include "interface.input.h"


// C Header
using namespace std;

class _checkbox : public _gadget , public _interface_input {
	
	private:
		
		bool pressed;
		
		static const _bitmap graphic[3];
		
		static _callbackReturn refreshHandler( _event event );
		
		static _callbackReturn mouseHandler( _event event );
		
		static _callbackReturn dragHandler( _event event );
		
	public:
		
		//! setIntValue
		void setIntValue( _s32 val ){ _interface_input::setIntValue( val ); this->bubbleRefresh( true ); }
		
		//! Constructor with dimsnions, coordinates, title and optional: Style
		_checkbox( _coord x , _coord y , _style style = _style() );
		
		//! Destrucor
		~_checkbox();
};

#endif