#ifndef _WIN_T_SCENARIOSETUP_
#define _WIN_T_SCENARIOSETUP_

#include "_type/type.scenario.h"
#include "_type/type.time.h"
#include "_type/type.gadget.h"
#include "_type/type.radiogroup.h"

class _scSetup : public _scenario
{
	private:
		
		int 			state;
		_gadget*		gadgets[20];
		_radiogroup*	radiogroup;
		string			profileName;
		_u8				profileIcon;
		_time			systemTime;
		
		_callbackReturn refreshStateHandler( _event e );
		_callbackReturn languageSelectHandler( _event e );
		_callbackReturn timeCounterHandler( _event e );
		_callbackReturn profileNameTextboxHandler( _event e );
		_callbackReturn stateChangeButtonHandler( _event e );
		_callbackReturn imagegadgetProfileIconHandler( _event e );
		
		//! Function to remove all gadgets
		void destruct( bool removeLanguagSelect = true );
	
	public:
		
		// Ctor
		_scSetup();
		
		// Dtor
		~_scSetup();
};

#endif