#ifndef _WIN_C_INTERRUPT_
#define _WIN_C_INTERRUPT_

#include <_type/type.h>
#include <_type/type.controller.h>

class _interruptController : public _controller
{
	private:
		
		static int criticalCount;
		
		//! Handler for the verical blank interrupt
		static void vblHandler();
		
	public:
		
		//! Inits the interrupt system
		static bool init();
		
		//! Disables/enables the VBlank interrupt
		//! to not be interrupted during an important process
		static void enterCriticalSection();
		static void leaveCriticalSection();
		
		//! Waits for the Veertical blank/count interrupt to occour
		static void waitForVerticalBlank( bool skipIfAlreadyOccoured = false );
		static void waitForVerticalCount( bool skipIfAlreadyOccoured = false );
		
		//! Get the current updated scanline
		static _u16 getCurrentScanline();
};

#endif