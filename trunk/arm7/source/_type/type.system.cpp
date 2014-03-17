#include "nds/arm7/audio.h"

#include "_type/type.system.h"

#include <time.h>
#include <nds/timers.h>
#include <nds/interrupts.h>
#include <nds/system.h>
#include <nds/arm7/input.h>
#include <nds/input.h>
#include <nds/arm7/clock.h>
#include <nds/fifocommon.h>
#include <dswifi7.h>

//const _u32 _system::ticksPerSecond = ( BUS_CLOCK + 32768 ) >> 10; // Works for about 5 Minutes (>5min.: buffer underrun)
const _u32 _system::ticksPerSecond = 65521; // Tested and found to work >6min

//! the speed in which the timer ticks in hertz.
//! #define BUS_CLOCK (33513982)
//! BUS_CLOCK >> 15 - BUS_CLOCK >> 21 - BUS_CLOCK >> 22 = 1000 * rawTime
//! There you get milliseconds from rawTime!

_tempTime _system::getBUSTime()
{
	// Check Timers
	_u64 lo = TIMER_DATA(0);
	_u64 mi = TIMER_DATA(1);
	_u64 hi = TIMER_DATA(2);
	
	_u64 lo2 = TIMER_DATA(0);
	_u64 mi2 = TIMER_DATA(1);
	_u64 hi2 = TIMER_DATA(2);

	if ( lo2 < lo )
	{
		lo = lo2;
		hi = hi2;
		mi = mi2;
	}
	
	return lo<<1 | mi<<17 | hi << 32;
}

namespace{
	static int criticaly = 1;
}

void _system::enterCriticalSection()
{
	if( --criticaly <= 0 )
		irqDisable( IRQ_VBLANK ); // Enter critical Section
}

void _system::leaveCriticalSection()
{
	if( ++criticaly > 0 )
		irqEnable( IRQ_VBLANK ); // Leave critical Section
}

void _system::vcntHandler(){
	inputGetAndSend();
}

void _system::vblHandler(){
	Wifi_Update();
}

void _system::powerBtnHandler(){
	_system::exitMain = true;
}

void _system::init()
{
	//! Power on everything 
	powerOn( POWER_ALL );
	
	// ------------------------------------------------------------------------
	// Init System
	// ------------------------------------------------------------------------
	
		//! Read the DS users personal information from the firmware 
		readUserSettings();
		
		//! Init Interrupt Routines
		irqInit();
		
		//! Start the RTC tracking IRQ
		initClockIRQ();
	
	// ------------------------------------------------------------------------
	// FIFO
	// ------------------------------------------------------------------------
	
		//! Enable FIFO
		fifoInit();
		
		//! Install all FIFO Handlers
		installWifiFIFO();
		installSystemFIFO();
		_sound::installFIFO();
	
	// ------------------------------------------------------------------------
	// Interrupts
	// ------------------------------------------------------------------------
	
		//! Set up interrupt handlers
		SetYtrigger( 192 );
		irqSet( IRQ_VBLANK , _system::vblHandler );
		irqSet( IRQ_VCOUNT , _system::vcntHandler );
		irqEnable( IRQ_VBLANK | IRQ_VCOUNT | IRQ_NETWORK );
		
		//! Set callback to be called at the press of the DSi's Power Button
		setPowerButtonCB( _system::powerBtnHandler );   
		
		//! Start Timers
		TIMER_CR(0) = 0;
		TIMER_CR(1) = 0;
		TIMER_CR(2) = 0;

		TIMER_DATA(0) = 0;
		TIMER_DATA(1) = 0;
		TIMER_DATA(2) = 0;

		TIMER_CR(2) = TIMER_CASCADE | TIMER_ENABLE;
		TIMER_CR(1) = TIMER_CASCADE | TIMER_ENABLE;
		TIMER_CR(0) = TIMER_ENABLE | ClockDivider_1024;
}

void _system::main()
{
	static const _u16 keyPattern = KEY_SELECT|KEY_START|KEY_L|KEY_R;
	
	while( !_system::exitMain )
	{
		if( ( REG_KEYINPUT & keyPattern ) == 0 )
			_system::exitMain = true;
		
		_sound::runSounds();
		
		// Wait until VBLANK occours or already had occoured
		swiIntrWait( 1 , IRQ_VBLANK );
	}
}

volatile bool _system::exitMain = false;