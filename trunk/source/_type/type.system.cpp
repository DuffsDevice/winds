#include "_type/type.system.h"
#include <nds.h>
#include <time.h>
#include <array>
#include "stdio.h"
#include <nds/timers.h>

#define transfer (*(__TransferRegion volatile *)(0x02FFF000))

struct __TransferRegion {
	vs16 touchX,   touchY;		// TSC X, Y
	vs16 touchXpx, touchYpx;	// TSC X, Y pixel values
	vs16 touchZ1,  touchZ2;		// TSC x-panel measurements
	vu16 buttons;				// X, Y, /PENIRQ buttons
	time_t	unixTime;
	struct __bootstub *bootcode;
};

void _system::setBacklight( _u8 level ){
	
}

void _system::restart(){
	struct __bootstub *bootcode = transfer.bootcode;
	bootcode->arm9reboot();
}

void _system::shutDown(){
	systemShutDown(); 
}

void _system::debug( string msg ){
	time_t rawtime = time(NULL);
	struct tm* t = localtime( &rawtime );
	_system::_debugFile_->writeString( asctime( t ) + msg + "\n" );
}

bool _system::initFat(){
	return fatInitDefault();
}

void _system::_vblank_(){
	_system::_animations_.step();
	_system::processInput();
}

void _system::addEvent( _gadgetEvent event )
{
	if( _system::eventThrowable )
		_system::events.push_back( event );
	else 
		_system::newEvents.push_back( event );
}

void _system::processEvents()
{
	//optimizeEvents();
	// Do not throw any Events until we finished iterating through Events!!!!
	// -> This was a big Problem - Hours of finding that out!
	disableEventThrowing();
	
	// Temp...
	_gadget* gadget;
	_gadgetEventReturnType ret;

	for( _gadgetEvent event : _system::events )
	{
		gadget = (_gadget*)event.getArgs().getDestination();		
		
		// Default: Say its not handled
		// It can be set if Gadget != nullptr
		ret = not_handled;
		
		// Make the Gadget ( if one is specified ) react on the event
		if( gadget != nullptr )
			ret = gadget->handleEvent( event );
		
		
		// Check if the Event was Handled
		// -> If it can be deleted afterwards: Delete It!!!
		if( ret == not_handled_no_delete || ret == handled_no_delete )
			_system::_windows_->generateEvent( event );
	}

	// Erase all Events
	_system::events.clear();
	
	
	// Copy all new Events into events Vector...
	enableEventThrowing();
	
}

void _system::enableEventThrowing( void )
{
	// Copy new Events
	_system::events.insert( _system::events.end() , _system::newEvents.begin() , _system::newEvents.end() );
	
	// Erase temporary events
	_system::newEvents.clear();	
	
	// Reenable Events!
	_system::eventThrowable = true;
}

_u16 _system::getCurrentKeys(){
	return keysHeld() & (~(KEY_TOUCH|KEY_LID));
}

void _system::disableEventThrowing( void )
{
	// Disable Throwing of Events!
	_system::eventThrowable = false;
}

void _system::processInput()
{
	// Refresh Input
	scanKeys();
	
	
	// Touch
	static touchPosition startTouch; // For Dragging
	static touchPosition lastTouch;
	static touchPosition touchBefore;
	static _u32 touchCycles = 0;
	static _u8 cyclesLastClick = 0;
	static bool touchDragging = false;
	// Keys
	static _u16 lastKeys = 0; // 0 = No Keys
	static _u32 heldCycles[16] = {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	static _u16 bits[16] = {
			1, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7,
			1 << 8, 1 << 9, 1 << 10, 1 << 11, 1 << 12, 1 << 13,
			1 << 14, 1 << 15 };
	
	
	// Temp...
	_gadgetEventArgs args;
	_u16 newKeys = keysHeld();
	touchPosition newTouch;
	touchRead( &newTouch );
	
	if( cyclesLastClick && !( cyclesLastClick >> 7 ) )
		cyclesLastClick++;
	
	/*!
	 * Just Handle the Buttons, not the Pen!
	 **/
	for( u8 i = 0 ; i < 16 ; ++i )
	{
		//! Again: We do not handle Pen (as well as the lid)
		if( BIT(i) == KEY_TOUCH || BIT(i) == KEY_LID ) continue;
		
		
		// Increase Cycles
		if( GETBIT( newKeys , i ) )
		{
			if( heldCycles[i] == 0 )
			{
				// Set the Args
				args.reset();
				args.setKeyCode( BIT(i) );
				args.setCurrentKeyCodes( lastKeys );
				
				
				// Trigger the Event
				_system::_windows_->handleEvent( _gadgetEvent( keyDown , args ) );
			}
			
			
			// Increase Cycles
			heldCycles[i]++;
		}
		
		
		// Erase Cycles (but only if the Button of the Bit was pressed previously!)
		else if( heldCycles[i] > 0 )
		{
			// Set the Args
			args.reset();
			args.setKeyCode( bits[i] );
			args.setCurrentKeyCodes( lastKeys );
			
			
			// Trigger the Event
			_system::_windows_->handleEvent( _gadgetEvent( keyUp , args ) );
			
			
			// If keyup is fast enough, trigger keyClick (only if the "button" wasn't the mouse
			if( heldCycles[i] < _system::_runtimeAttributes_->maxClickCycles )
				_system::_windows_->triggerEvent( _gadgetEvent( keyClick , args ) );
			
			
			// Reset Cycles
			heldCycles[i] = 0;
		}
	}

	/*!
	 * Now we'll handle the Pen!
	 **/
	if( newKeys & KEY_TOUCH )
	{
		
		//! Check if this is the first Cycle the pen is down
		if( touchCycles == 0 )
		{
			// Set the Args
			args.reset();
			args.setCurrentKeyCodes( newKeys );
			args.setPosX( newTouch.px );
			args.setPosY( newTouch.py );
			
			
			// Trigger the Event
			_system::_windows_->handleEvent( _gadgetEvent( mouseDown , args ) );
			
			
			// Set the starting Point of the mouseDown here!
			startTouch = newTouch;
			lastTouch = newTouch;
		}

		//! Check if the Pen is dragging something
		if( touchDragging )
		{
			// Set the Args
			args.reset();
			args.setCurrentKeyCodes( newKeys );
			args.setDeltaX( newTouch.px - lastTouch.px );
			args.setDeltaY( newTouch.py - lastTouch.py );
			args.setPosX( newTouch.px );
			args.setPosY( newTouch.py );
			
			// Trigger the Event
			_system::_windows_->handleEvent( _gadgetEvent( dragging , args ) );
			
			lastTouch = newTouch;
		}

		//! Check if the Stylus has moved
		if( !touchDragging )
		{
			// Compute the dragged Distance
			// -> Pythagoras!
			u32 xd = newTouch.px - startTouch.px;
			u32 yd = newTouch.py - startTouch.py;
			u32 dragDistance = xd * xd + yd * yd;
			
			
			// Check if Pen has moved the distance already
			if( dragDistance > _system::_runtimeAttributes_->minDragDistance * _system::_runtimeAttributes_->minDragDistance )
			{
				// Set the Args
				args.reset();
				args.setCurrentKeyCodes( newKeys );
				args.setPosX( startTouch.px );
				args.setPosY( startTouch.py );
				
				// Trigger the Event!
				_system::_windows_->handleEvent( _gadgetEvent( dragStart , args ) );
				
				lastTouch = startTouch;
				
				// Set touchDragging to true!
				touchDragging = true;
				
			}
		}
		
		
		// Increase Cycles
		touchCycles++;
	}
	//! Pen is not down
	else if( touchCycles > 0 )
	{
		// Set the Args
		args.reset();
		args.setCurrentKeyCodes( newKeys );
		args.setPosX( lastTouch.px );
		args.setPosY( lastTouch.py );
		
		
		//! Maybe DragStop-Event?
		if( touchDragging )
		{
			// Set touchDragging to false!
			touchDragging = false;
			
			// Trigger the Event!
			_system::_windows_->handleEvent( _gadgetEvent( dragStop , args ) );
		}
		//! Maybe Click-Event?
		else if( touchCycles < _system::_runtimeAttributes_->maxClickCycles )
		{
			_s16	deltaX = touchBefore.px - lastTouch.px;
			_s16	deltaY = touchBefore.py - lastTouch.py;
			_s16 	deltaTouch = deltaX * deltaX + deltaY * deltaY;
			// Trigger the mouseClick-Event!
			if( cyclesLastClick && cyclesLastClick < _system::_runtimeAttributes_->maxDoubleClickCycles && deltaTouch < _system::_runtimeAttributes_->maxDoubleClickArea * _system::_runtimeAttributes_->maxDoubleClickArea ){
				_system::_windows_->handleEvent( _gadgetEvent( mouseDoubleClick , args ) );
				cyclesLastClick = 0;
			}
			else{
				_system::_windows_->handleEvent( _gadgetEvent( mouseClick , args ) );
				cyclesLastClick = 1;
			}
		}
		
		// Trigger the mouseUp-Event!
		_system::_windows_->handleEvent( _gadgetEvent( mouseUp , args ) );
		touchBefore = lastTouch;
		
		// Reset touch Cycles
		touchCycles = 0;
		
	}

	// Refresh the old inputs
	lastKeys = newKeys;
}

_system::_system()
{
	//! Power on everything 
	powerOn( POWER_ALL );
	
	//! Set the video mode on the main screen.
	videoSetMode( MODE_5_2D );
	
	//lcdMainOnTop();
	lcdMainOnBottom();
	
	//! Set the video mode on the sub screen.
	videoSetModeSub( MODE_5_2D );
	
	//! Init Video RAMs
	vramSetBankA( VRAM_A_MAIN_BG );
	vramSetBankC( VRAM_C_SUB_BG );
	
	//! Init Backgrounds
	bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	
	consoleDemoInit();
	
	//! Set the VBLANK Interrupt handler
	irqSet( IRQ_VBLANK , _system::_vblank_ );
	
	// Create RTA
	_system::_runtimeAttributes_ = new _runtimeAttributes;
	
	// Make sure there is a file to debug to 
	_system::_debugFile_ = new _file("/%WINDIR%/debug.txt");
	//_system::_debugFile_->create( true );
	
	cpuStartTiming(1);
}

void _system::benchMarkStart(){
	cpuStartTiming(true);
}

void _system::benchMarkStopPrint(){
	printf("Ticks:%d",cpuGetTiming() );
	cpuEndTiming();
	submit();
}

void _system::submit(){
	while( !(keysDown() & KEY_A) )
		scanKeys();
	while( keysDown() & KEY_A )
		scanKeys();
}

_u32 _system::getNow(){
	return cpuGetTiming()>>15;
}

void _system::setWindows( _windows* win ){
	_system::_windows_ = win;
}

void _system::addAnimation( _animation<int>* anim ){
	_system::_animations_.add( anim );
}

void _system::addProgram( _program* prog ){
	_system::_programs_.push_back( prog );
	prog->run( _system::_windows_ );
}

vector<char> t;

void _system::displayMemUsage(){
	printf( "%d\n" , t.max_size() );
}

void _system::run(){
	while(true){
		_system::processEvents();
		//displayMemUsage();
		swiWaitForVBlank();
	}
}

void _system::runPrograms(){
	for( _program* prog : _system::_programs_ )
		prog->run( _system::_windows_ );
};

_system* _system_ = new _system;


// Static Attributes...

bool 					_system::sleeping = false;
_animationsGroup<int> 	_system::_animations_;
deque<_program*> 		_system::_programs_;
_windows*				_system::_windows_ = nullptr;
_runtimeAttributes*		_system::_runtimeAttributes_ = nullptr;
_file*					_system::_debugFile_ = nullptr;
//! Events
deque<_gadgetEvent> 	_system::events;
deque<_gadgetEvent> 	_system::newEvents;
bool _system::eventThrowable = true;