#ifndef _WIN_T_WINDOWS_
#define _WIN_T_WINDOWS_

#include <_type/type.predefines.h>
#include <_type/type.sound.h>

class _windows{
	
	private:
		
		static volatile bool exitMain;
		
		//! Disables/enables the VBlank interrupt
		//! to not be interrupted during an important process
		static void enterCriticalSection();
		static void leaveCriticalSection();
		
		//! Handlers for specific interrupts
		static void vcntHandler();
		static void vblHandler();
		static void powerBtnHandler();
		
		//! Sends cpuUsage to arm9
		static void sendCpuUsage( _u8 percent );
		
		//! Temporary variable
		static int cpuUsageTemp;
		
	public:
		
		//! Constructor
		static void			init();
		
		//! Get Current Time (milliseconds since system startup)
		static _tempTime	getBUSTime();
		
		//! main Loop...
		static void			main();
		
		//! The BUS-Clock-Frequency (number of ticks per second)
		static const _u32	ticksPerSecond;
		
		//! Send a debug message
		static void			debug( _literal message );
};

#endif