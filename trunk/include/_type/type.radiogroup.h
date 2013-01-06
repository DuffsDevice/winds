#ifndef _WIN_T_RADIOGROUP_
#define _WIN_T_RADIOGROUP_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include "_gadget/gadget.radio.h"

class _radiogroup{
	
	private:
		
		_radio*	activeRadio;
		map<_s32,_radio*> radios;
		
	public:
		
		/**
		 * Add a _radio to the list
		 * @return the Index to identify the radio later
		**/
		_s32 addRadio( _radio* radio );
		
		void removeRadio( _radio* radio );
		
		void enableRadio( _radio* );
		
		void setIntValue( _s32 val );
		
		_s32 getIntValue();
		
		_radio*	getActiveRadio(){ return this->activeRadio; }
		
};

#endif