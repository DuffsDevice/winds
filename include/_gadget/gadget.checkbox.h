// Check if already included
#ifndef _WIN_G_CHECKBOX_
#define _WIN_G_CHECKBOX_

#include "_type/type.gadget.h"
#include "_gadget/gadget.label.h"

class _checkbox : public _gadget {
	
	private:
		
		bool	pressed;
		
		_u8		intValue;
		
		static const _bitmap graphic[3];
		
		static _callbackReturn refreshHandler( _event event );
		
		static _callbackReturn mouseHandler( _event event );
		
		static _callbackReturn dragHandler( _event event );
		
	public:
		
		//! setIntValue
		void setIntValue( _s32 val ){ this->intValue = val; this->bubbleRefresh( true ); }
		
		//! getIntValue
		_int getIntValue(){ return this->intValue; }
		
		//! Constructor with dimsnions, coordinates, title and optional: Style
		_checkbox( _coord x , _coord y , _style style = _style() );
		
		//! Destrucor
		~_checkbox();
};

#endif