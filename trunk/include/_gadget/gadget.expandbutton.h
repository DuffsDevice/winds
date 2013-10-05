// Check if already included
#ifndef _WIN_G_EXPANDBUTTON_
#define _WIN_G_EXPANDBUTTON_

#include "_type/type.gadget.h"

class _expandButton : public _gadget {
	
	private:
		
		_u8		intValue;
		
		static _constbitmap graphic;
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn mouseHandler( _event );
		
	public:
		
		//! setIntValue
		void setIntValue( _s32 val ){ if( this->intValue == val ) return; this->intValue = val; this->redraw(); }
		
		//! getIntValue
		_int getIntValue(){ return this->intValue; }
		
		//! Constructor with dimensions, coordinates, title and optional: Style
		_expandButton( _coord x , _coord y , _style&& style = _style() );
};

#endif