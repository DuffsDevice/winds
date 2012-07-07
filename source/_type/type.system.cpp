#include "_type/type.system.h"
#include "func.memory.h"
#include <nds.h>
#include <time.h>
#include "stdio.h"
#include <nds/timers.h>
#include <dswifi9.h>
#include <fat.h>

//! Resources we need
#include "_resource/PROG_Explorer.h"
#include "_resource/BMP_Checkboxes.h"

#include "_gadget/gadget.windows.h"
#include "_gadget/gadget.startupScreen.h"

#define transfer (*(__TransferRegion volatile *)(0x02FFF000))

struct __TransferRegion {
	vs16 touchX,   touchY;		// TSC X, Y
	vs16 touchXpx, touchYpx;	// TSC X, Y pixel values
	vs16 touchZ1,  touchZ2;		// TSC x-panel measurements
	vu16 buttons;				// X, Y, /PENIRQ buttons
	time_t	unixTime;
	struct __bootstub *bootcode;
};

/*void _system::restart(){
	struct __bootstub *bootcode = transfer.bootcode;
	bootcode->arm9reboot();
}*/

void _system::shutDown()
{
	delete _system_;
}

void _system::debug( string msg ){
	time_t rawtime = time(NULL);
	struct tm* t = localtime( &rawtime );
	_system::_debugFile_->writeString( asctime( t ) + msg + "\r\n" );
	printf( "%s" , (asctime( t ) + msg + "\n").c_str() );
}

void _system::_vblank_()
{
	_system::processInput();
	//_system::_keyboard_->screenVBL();
}

void _system::generateEvent( _gadgetEvent event )
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
	
	//printf("%d\n",_system::events.size() );
	
	// Temp...
	_gadget* gadget;

	for( _gadgetEvent event : _system::events )
	{
		gadget = (_gadget*)event.getArgs().getDestination();		
		
		// Make the Gadget ( if one is specified ) react on the event
		if( gadget != nullptr )
			gadget->handleEvent( event );
		
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
	
	touchPosition t;
	touchRead( &t );

	// Temp...
	static _u16 lastKeys = 0; // 0 = No Keys
	static _u32 heldCycles[16] = {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	
	_gadgetEventArgs args;
	
	// Useful Values...
	static _u32 kRD = _system::_runtimeAttributes_->user->getIntAttr( "keyRepetitionDelay" );
	static _u32 kRS = _system::_runtimeAttributes_->user->getIntAttr( "keyRepetitionSpeed" );
	static _u32 mCC = _system::_runtimeAttributes_->user->getIntAttr( "maxClickCycles" );
	
	_key keys = keysHeld();
	
	if( !_system::_keyboard_->processTouch( keys & KEY_TOUCH , t ) )
		_system::_gadgetHost_->processTouch( keys & KEY_TOUCH , t );
	
	/*!
	 * Just Handle the Buttons, not the Pen!
	 **/
	for( _u8 i = 0 ; i < 16 && _system::_currentFocus_; ++i )
	{
		//! Again: We do not handle Pen (as well as the lid)
		if( BIT(i) == KEY_TOUCH || BIT(i) == KEY_LID ) continue;
		
		
		// Increase Cycles
		if( GETBIT( keys , i ) )
		{
			if( heldCycles[i] == 0 || ( kRD && heldCycles[i] > kRD && heldCycles[i] % kRS == 0 ) )
			{
				// Set the Args
				args.reset();
				args.setKeyCode( libnds2key[i] );
				args.setCurrentKeyCodes( lastKeys );
				
				// Trigger the Event
				_system::_currentFocus_->handleEvent( _gadgetEvent( "keyDown" , args ) );
			}
			
			// Increase Cycles
			heldCycles[i]++;
		}
		// Erase Cycles (but only if the Button of the Bit was pressed previously!)
		else if( heldCycles[i] > 0 )
		{
			// Set the Args
			args.reset();
			args.setKeyCode( libnds2key[i] );
			args.setCurrentKeyCodes( lastKeys );
			
			// Trigger the Event
			_system::_currentFocus_->handleEvent( _gadgetEvent( "keyUp" , args ) );
			
			// If keyup is fast enough, trigger keyClick (only if the "button" wasn't the mouse
			if( heldCycles[i] < mCC )
				_system::_currentFocus_->handleEvent( _gadgetEvent( "keyClick" , args ) );
			
			// Reset Cycles
			heldCycles[i] = 0;
		}
	}
}

_system::_system()
{
	// ------------------------------------------------------------------------
	// Display 
	// ------------------------------------------------------------------------
	
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
		vramSetBankB( VRAM_B_MAIN_BG );
		vramSetBankC( VRAM_C_SUB_BG);
		vramSetBankD( VRAM_D_MAIN_BG );
		
		//! Init Backgrounds
		_system::bgIdFront = bgInit(2, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
		_system::bgIdBack = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 8, 0);
		_system::bgIdSub = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
		
		//setBackdropColor( COLOR_RED );
		consoleDemoInit();
		//printf("%d\n",1<<15);
		//consoleInit	( nullptr , 1 , BgType_Text4bpp , BgSize_T_256x256 , 31 , 0 , true , true );
		
	// ------------------------------------------------------------------------
	// Interrupts
	// ------------------------------------------------------------------------
	
		//! Set the VBLANK Interrupt handler
		irqSet( IRQ_VBLANK , _system::_vblank_ );
		
		//! Start Time
		cpuStartTiming(1);
		
		//return;
	
	// ------------------------------------------------------------------------
	// System-Attributes
	// ------------------------------------------------------------------------
		
		// Create RTA
		//_system::_runtimeAttributes_->wallpaper = new BMP_WindowsWallpaper();
		_system::_runtimeAttributes_ = new _runtimeAttributes;
		_system::_runtimeAttributes_->user = new _user("Jakob");
		
		// Make sure there is a file to debug to
		_system::_debugFile_ = new _direntry("/%WINDIR%/debug.txt");
		_system::_debugFile_->create();
		
		_system::_registry_ = new _registry("/%WINDIR%/windows.reg");
		
		//! Create Windows & the Keyboard
		//_system::_gadgetHost_ = new _windows( bgIdBack );
		_system::_topScreen_ = new _screen( bgIdSub );
		//_system::_gadgetHost_ = new _windows( bgIdBack );
		_system::_gadgetHost_ = new _startupScreen( bgIdBack );
		_system::_keyboard_ = new _keyboard( bgIdFront , _system::_gadgetHost_ , _system::_topScreen_ );
		
		//! random Random() generator
		srand( time(NULL) );
		
	
	// ------------------------------------------------------------------------
	// Wifi & FAT32
	// ------------------------------------------------------------------------
		
		// TODO: Implement Wifi-Settings-App
		//! libWifi instantionation
		/*if( !Wifi_InitDefault(WFC_CONNECT) )
			_system::debug("Wifi could not be inited! Please not that the default Firmware Settings are used to connect");*/
		
	
	// ------------------------------------------------------------------------
	
}

_system::~_system()
{
	delete _system::_registry_;
	delete _system::_debugFile_;
	delete _system::_keyboard_;
	delete _system::_gadgetHost_;
	delete _system::_runtimeAttributes_->user;
	delete _system::_runtimeAttributes_;
	systemShutDown();
}

void _system::runAnimations(){
	start:
	for( auto animIter = _system::_animations_.begin() ; animIter != _system::_animations_.end() ; animIter++ )
	{			
		if( animIter->finished() )
		{
			_system::_animations_.erase( animIter );
			goto start;
		}
		animIter->step( _system_->getTime() );
	}
}

void _system::submit(){
	while( !(keysDown() & KEY_A) )
		scanKeys();
	while( keysDown() & KEY_A )
		scanKeys();
}

_u32 _system::getTime(){
	return cpuGetTiming()>>15;
}

void _system::executeAnimation( const _animation& anim ){
	_system::_animations_.remove_if( [&]( _animation& a )->bool{ if( a.getID() == anim.getID() ) { return true; } return false; } );
	_system::_animations_.push_back( anim );
}

void _system::executeProgram( _program* prog , _cmdArgs args ){
	_system::_programs_.push_back( make_pair( prog , args ) );
	prog->init( _system::_gadgetHost_ , args );
}

void _system::main(){
	//_direntry d = _direntry("hallo.exe");
	//d.execute();
	SetYtrigger( 192 );
	irqEnable( IRQ_VCOUNT );
	//int i = 0;
	while(true)
	{
		// wait until line 0
		//swiIntrWait( 192, IRQ_VCOUNT );
		
		//printf( "%d\n" , REG_VCOUNT );
		_system::processEvents();
		_system::runAnimations();
		_system::runPrograms();
		BG_PALETTE_SUB[0] = RGB( 20 , 20 , 20 );
		swiWaitForVBlank();
		//consoleClear();
		//BG_PALETTE_SUB[0] = RGB( 31 , 31 , 31 );
		/*if( ++i > 6000 ){
			delete _system_;
			return;
		}*/
	}
}

_program* _system::getBuiltInProgram( string qualifiedName ){
	if( qualifiedName == "explorer.exe" ){
		return new PROG_Explorer();
	}
	return nullptr;
}

void _system::runPrograms()
{
	start:
	for( auto progIter = _system::_programs_.begin(); progIter != _system::_programs_.end() ; progIter++ )
	{
		if( progIter->first->main( progIter->second ) == 1 ){
			if( progIter->first->autoDelete )
				delete progIter->first;
			_system::_programs_.erase( progIter );
			goto start;
		}
	}
};


//! Static Attributes...
bool 							_system::sleeping = false;
list<_animation>				_system::_animations_;
list<pair<_program*,_cmdArgs>>	_system::_programs_;
_gadgetScreen*					_system::_gadgetHost_ = nullptr;
_screen*						_system::_topScreen_ = nullptr;
_keyboard*						_system::_keyboard_ = nullptr;
_registry*						_system::_registry_ = nullptr;
_runtimeAttributes*				_system::_runtimeAttributes_ = nullptr;
_direntry*						_system::_debugFile_ = nullptr;
_gadget*						_system::_currentFocus_ = nullptr;
int								_system::bgIdFront;
int								_system::bgIdBack;
int								_system::bgIdSub;

//! Events
list<_gadgetEvent>				_system::events;
list<_gadgetEvent> 				_system::newEvents;
bool 							_system::eventThrowable = true;

// Static system...
_system* _system_ = new _system();