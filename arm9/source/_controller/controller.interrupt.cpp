#include "_controller/controller.interrupt.h"
#include "_controller/controller.debug.h"
#include "_type/type.windows.h"

#include <nds/interrupts.h>
#include <nds/bios.h>
#include <nds/system.h>

bool _interruptController::init()
{
	// Reset stack count
	_interruptController::criticalCount = 0;
	
	// Set the number of scanlines after which the Vertical Count Interrupt is fired
	SetYtrigger( 0 );
	
	// Set handler for Vertical Blank Interrupt
	irqSet( IRQ_VBLANK , &_interruptController::vblHandler );
	
	// Enable Interrupts
	irqEnable( IRQ_VBLANK | IRQ_VCOUNT );
	
	return true;
}

void _interruptController::vblHandler(){
	_windows::interrupt();
}

_u16 _interruptController::getCurrentScanline(){
	return REG_VCOUNT;
}

void _interruptController::enterCriticalSection()
{
	++ _interruptController::criticalCount;
	irqDisable( IRQ_VBLANK ); // Enter critical Section
}

void _interruptController::waitForVerticalBlank( bool skipIfAlreadyOccoured ){
	swiIntrWait( !skipIfAlreadyOccoured  , IRQ_VBLANK );
}

void _interruptController::waitForVerticalCount( bool skipIfAlreadyOccoured ){
	swiIntrWait( !skipIfAlreadyOccoured , IRQ_VCOUNT );
}

void _interruptController::leaveCriticalSection()
{
	if( -- _interruptController::criticalCount == 0 )
		irqEnable( IRQ_VBLANK ); // Leave critical Section
}

int _interruptController::criticalCount = 0;