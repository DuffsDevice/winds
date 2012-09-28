// Check if already included
#ifndef _WIN_G_RADIO_
#define _WIN_G_RADIO_

#include "_type/type.gadget.h"
#include "interface.input.h"

class _radiogroup;

class _radio : public _gadget , public _interface_input {
	
	private:
		
		bool pressed;
		
		_radiogroup* radiogroup;
		
		static const _bitmap graphic[4];
		
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
		static _gadgetEventReturnType mouseHandler( _gadgetEvent event );
		static _gadgetEventReturnType dragHandler( _gadgetEvent event );
		
	public:
		
		//! setIntValue
		void setIntValue( _s32 val ){ _interface_input::setIntValue( bool(val) ); this->bubbleRefresh( true ); }
		
		//! Constructor with dimsnions, coordinates, title and optional: Style
		_radio( _coord x , _coord y , _radiogroup* radiogroup , _gadgetStyle style = _defaultStyle_ );
		
		//! Destrucor
		~_radio();
};

#include "_type/type.radiogroup.h"

#endif