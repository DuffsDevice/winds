#ifndef _WIN_T_WINDOWS_
#define _WIN_T_WINDOWS_

#include <_type/type.h>
#include <_type/type.controller.h>
#include <_type/type.language.h>
#include <_type/type.program.handle.h>

//! Enumerates the underlying hardware
enum class _hardwareType{
	emulator,
	ds,
	dsi
};
extern _toStr<_hardwareType> hardwareType2str;

class _winds
{
	private:
		
		//! Collection of necessary init methods in their chronological order
		static _vector<_pair<_controllerInit*,string>>	initMethods;
		static _vector<_controllerFrame*>				mainMethods;
		static _vector<_controllerFrame*>				interruptMethods;
		static _vector<_controllerEnd*>					endMethods;
		
		//! Allows tracking of the processor usage
		static int 					cpuUsageTemp;
		static int 					cpuUsageTempSub;
		
		//! Flag indicating whether end() was not called after an initial call to init()
		static bool					active;
		
		//! Flag that idicates whether the main() routine should stop looping
		static bool					terminateMain;
		
		//! For debug purposes only
		static void					displayMemoryUsage();
		
		//! DMA Handler for messages from the arm7
		static void					fifoDataHandler( _s32 bytes , void* userData );
		
	public:
		
		//! Init routine that inits all other controllers
		static void					init();
		
		//! Routine that calls all controllors that want to be called during the vertical blank interrupt
		static void					interrupt();
		
		//! Routine that calls all controllers that have to be called as often as possible
		static void					main();
		
		//! Routine that shuts down all controllers
		static void					end();
		
		//! Execute the supplied command and eventually get back a handle to the program
		static _programHandle		execute( const string& cmd );
		
		//! Get current Cpu-usage
		static _u8					getCpuUsage(){ return cpuUsageTemp; }
		static _u8					getCpuUsageSub(){ return cpuUsageTempSub; }
		
		//! Get the name of the user within the DS internal firmware
		static const wstring&		getDSUserName();
		
		//! GEt the DS internal language used by the firmware
		static _language			getDSLanguage();
		
		//! Check, on which hardware type the executeable is running on
		static _hardwareType		getHardwareType();
		
		//! Stops the execution of main() if main() was already called
		static void					stop();
};

#endif