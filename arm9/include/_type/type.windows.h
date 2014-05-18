#ifndef _WIN_T_WINDOWS_
#define _WIN_T_WINDOWS_

#include "_type/type.h"
#include "_type/type.controller.h"
#include "_type/type.language.h"

//! Enumerates the underlying hardware
enum class _hardwareType{
	emulator,
	ds,
	dsi
};
extern _toStr<_hardwareType> hardwareType2str;

class _windows
{
	private:
		
		//! Collection of necessary init methods in their chronological order
		static _vector<_pair<_controllerInit*,string>>	initMethods;
		static _vector<_controllerFrame*>				mainMethods;
		static _vector<_controllerFrame*>				interruptMethods;
		static _vector<_controllerEnd*>					endMethods;
		
		//! Allows tracking of the processor usage
		static int 					cpuUsageTemp;
		
		//! For debug purposes only
		static void					displayMemoryUsage();
		
	public:
		
		//! Init routine that inits all other controllers
		static void					init();
		
		//! Routine that calls all controllors that want to be called during the vertical blank interrupt
		static void					interrupt();
		
		//! Routine that calls all controllers that have to be called as often as possible
		static void					main();
		
		//! Routine that shuts down all controllers
		static void					end();
		
		//! Execute the supplied command
		static bool					executeCommand( const string& cmd );
		
		//! Get current Cpu-usage
		static _u8					getCpuUsage(){ return cpuUsageTemp; }
		
		//! Get the name of the user within the DS internal firmware
		static const string&		getDSUserName();
		
		//! GEt the DS internal language used by the firmware
		static _language			getDSLanguage();
		
		//! Check, on which hardware type the executeable is running on
		static _hardwareType		getHardwareType();
};

#endif