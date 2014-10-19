// Check if already included
#ifndef _WIN_G_checkBox_
#define _WIN_G_checkBox_

#include <_type/type.gadget.h>

class _checkBox : public _gadget {
	
	private:
		
		_u8		intValue;
		
		static _constBitmap graphic[3];
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn mouseHandler( _event );
		
	public:
		
		//! setIntValue
		void setIntValue( _s32 val ){ if( this->intValue == val ) return; this->intValue = val; this->redraw(); }
		
		//! getIntValue
		_int getIntValue(){ return this->intValue; }
		
		//! Constructor with dimensions, coordinates, title and optional: Style
		_checkBox( _optValue<_coord> x , _optValue<_coord> y , _style&& style = _style() ) :
			_checkBox( x , y , 0 , (_style&&)style )
		{}
		_checkBox( _optValue<_coord> x , _optValue<_coord> y , _u8 value , _style&& style = _style() );
};

#endif