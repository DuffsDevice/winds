// Check if already included
#ifndef _WIN_G_RADIO_
#define _WIN_G_RADIO_

#include "_type/type.gadget.h"
#include "_type/type.singlevaluegroup.h"


class _radio : public _gadget {
	
	private:
		
		bool						intValue;
		_singleValueGroup<_radio>* 	group;
		
		static const _bitmap 		graphic[4];
		
		static _callbackReturn refreshHandler( _event event );
		static _callbackReturn mouseHandler( _event event );
		static _callbackReturn keyHandler( _event event );
		
		friend class _singleValueGroup<_radio>;
		
	public:
		
		//! setIntValue
		void setIntValue( _s32 val ){ this->intValue = val; this->redraw(); }
		
		//! getIntValue
		_s32 getIntValue(){ return this->intValue; }
		
		//! Constructor with dimsnions, coordinates, title and optional: Style
		_radio( _coord x , _coord y , _singleValueGroup<_radio>* radiogroup , _s32 assocValue = -1 , _style&& style = _style() );
		
		//! Destrucor
		~_radio();
};

#endif