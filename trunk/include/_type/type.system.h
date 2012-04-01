#ifndef _WIN_T_SYSTEM_
#define _WIN_T_SYSTEM_

#include "_type/type.h"
#include "fat.h"

class _system{
	
	private:
		
		bool sleeping;
		
	public:
		
		void goToSleep();
		
		void wakeUp();
		
		void shutDown();
		
		void restart();
		
		bool initFat();
		
		void initWifi();
		
		void initDisplay();
		
		void setBacklight( _u8 level );
};

#endif