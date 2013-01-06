// Check if already included
#ifndef _WIN_G_RADIO_
#define _WIN_G_RADIO_

#include "_type/type.gadget.h"

class _radiogroup;

class _radio : public _gadget {
	
	private:
		
		bool			pressed;
		bool			intValue;
		_radiogroup* 	radiogroup;
		
		static const _bitmap graphic[4];
		
		static _callbackReturn refreshHandler( _event event );
		static _callbackReturn mouseHandler( _event event );
		static _callbackReturn dragHandler( _event event );
		
		friend class _radiogroup;
		
	public:
		
		//! setIntValue
		void setIntValue( _s32 val ){ this->intValue = val; this->bubbleRefresh( true ); }
		
		//! getIntValue
		_s32 getIntValue(){ return this->intValue; }
		
		//! Constructor with dimsnions, coordinates, title and optional: Style
		_radio( _coord x , _coord y , _radiogroup* radiogroup , _s32 assocValue = -1 , _style style = _style() );
		
		//! Destrucor
		~_radio();
};

#include "_type/type.radiogroup.h"

#endif