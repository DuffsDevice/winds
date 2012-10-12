#include "_type/type.system.h"
#include "func.memory.h"
#include <nds.h>
#include <time.h>
#include "stdio.h"
#include <nds/timers.h>
#include <dswifi9.h>
#include <fat.h>

//! Resources we need
//#include "_resource/PROG_Explorer.h"
#include "_resource/BMP_Checkboxes.h"

#include "_gadget/gadget.windows.h"
#include "_gadget/gadget.startupScreen.h"
#include "_gadget/gadget.bootupScreen.h"

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

void _system::initializeComponents(){
	_bootupScreen* screen;
	_system::_gadgetHost_ = screen = new _bootupScreen( bgIdBack );
	
	// -----------------------------------------------
	//	Hardware Init
	// -----------------------------------------------
	
	bool fatInit = _direntry::initFat();
	bool wifiInit = Wifi_InitDefault(INIT_ONLY);
	
	if( fatInit && !wifiInit )
		_system::debug("Wifi could not be inited! Please note that the default Firmware Settings are used to connect");
		
	// -----------------------------------------------
	//	Software and Gadget-System-Init Init
	// -----------------------------------------------
	
		// Create RTA
		_system::_runtimeAttributes_ = new _runtimeAttributes;
		_system::_runtimeAttributes_->user = new _user("Jakob");
		
		// Make sure there is a file to debug to
		_system::_debugFile_ = new _direntry("/%WINDIR%/debug.txt");
		_system::_debugFile_->create();
		
		_system::_registry_ = new _registry("/%WINDIR%/windows.reg");
		
	//ende
	_system::_keyboard_ = new _keyboard( bgIdFront , _system::_gadgetHost_ , _system::_topScreen_ );
}
	

void _system::_vblank_()
{
	if( _system::_gadgetHost_ )
		_system::processInput();
	if( _system::_keyboard_ )
		_system::_keyboard_->screenVBL();
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

	for( _gadgetEvent& event : _system::events )
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
	
	
	/*if( !_system::events.size() )
	{
		addMethod( reinterpret_cast<void*>(&_memoryfont::drawCharacter),"fontDrawing");
		addMethod( reinterpret_cast<void*>(&_gadget::gadgetRefreshHandler),"refresh");
		addMethod( reinterpret_cast<void*>(&_bitmap::copyTransparent),"copyAlgo");
		addMethod( reinterpret_cast<void*>(&_rect::reduce),"reduceAlgo");
		addMethod( reinterpret_cast<void*>(&_rect::toRelative),"toRelative");
		
		printResults();
		printf("%d\n",cpuGetTiming());
		while(true);
	}*/
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
	
	// Shortcut...
	const _user* user = _system_->_runtimeAttributes_->user;
	
	_key keys = keysHeld();
	
	if( !_system::_keyboard_->processTouch( keys & KEY_TOUCH , t ) )
		_system::_gadgetHost_->processTouch( keys & KEY_TOUCH , t );
	
	if( keysDown() & KEY_SELECT )
	{
		if( _system::_keyboard_->isOpened() )
			_system::_keyboard_->close();
		else
			_system::_keyboard_->open();
	}
	
	/*!
	 * Just Handle the Buttons, not the Pen!
	 **/
	for( _u8 i = 0 ; i < 16 && _system::_currentFocus_ ; ++i )
	{
		//! Again: We do not handle Pen (as well as the lid)
		if( BIT(i) == KEY_TOUCH || BIT(i) == KEY_LID ) continue;
		
		
		// Increase Cycles
		if( GETBIT( keys , i ) )
		{
			if( heldCycles[i] == 0 || ( user->kRD && heldCycles[i] > user->kRD && heldCycles[i] % user->kRS == 0 ) )
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
			if( heldCycles[i] < user->mCC )
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
		
		lcdMainOnBottom();
		//lcdMainOnTop();
		
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
		
		//setBackdropColor( COLOR_BLACK );
		//setBackdropColorSub( COLOR_BLACK );
		//consoleDemoInit();
		
		//memSet( BG_GFX_SUB , 0 , SCREEN_HEIGHT*SCREEN_WIDTH );
		//consoleInit	( nullptr , 1 , BgType_Text4bpp , BgSize_T_256x256 , 31 , 0 , true , true );
		
	// ------------------------------------------------------------------------
	// Interrupts
	// ------------------------------------------------------------------------
	
		//! Set the VBLANK Interrupt handler
		irqSet( IRQ_VBLANK , _system::_vblank_ );
		
		//! Start Time
		cpuStartTiming(1);
		
		/*_bitmap bmp = _bitmap( BG_GFX , SCREEN_WIDTH , SCREEN_HEIGHT );
		
		_area b = { _rect( 40 , 40 , 40 , 40 ) , _rect( 50 , 50 , 40 , 40 ) };
		for( _rect rc : b )
			bmp.drawRect( rc.x , rc.y , rc.width , rc.height , RGB( 0 , 0 , 31 ) );
			
		_area a = _rect( 40 , 40 , 40 , 40 ).reduce(_rect( 50 , 50 , 40 , 40 ));
		
		int i = -6;
		for( _rect rc : a )
		{
			for( int i = 0; i < 60; i++ )swiWaitForVBlank();
			bmp.drawRect( rc.x , rc.y , rc.width , rc.height , RGB( 31 - (i+=6) , i , 15 ) );
		}
		
		while(true);*/
	
	// ------------------------------------------------------------------------
	// System-Attributes
	// ------------------------------------------------------------------------
		
		//! Create Windows & the Keyboard
		//_system::_gadgetHost_ = new _windows( bgIdBack );
		_system::_topScreen_ = new _screen( bgIdSub );
		//_system::_gadgetHost_ = new _startupScreen( bgIdBack );
		
		//! random Random() generator
		srand( time(NULL) );
		
		//! Set Memory-Alloc-Error-Handler
		set_new_handler( &_system::_newErrorFunc_ );
	// ------------------------------------------------------------------------
	
	
	initializeComponents();
}

void _system::_newErrorFunc_()
{
	//_system::blueScreen(121,"Not enough Memory!");
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

void _system::runAnimations()
{
	_system::_animations_.remove_if( [&]( _animation* anim )->bool{ anim->step( _system_->getTime() ); return anim->finished(); } );
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

void _system::executeAnimation( _animation* anim )
{
	_system::_animations_.remove_if( [&]( _animation* a )->bool{ return a == anim; } );
	_system::_animations_.push_back( anim );
}

void _system::terminateAnimation( const _animation* anim )
{
	_system::_animations_.remove_if( [&]( _animation* a )->bool{ return a == anim; } );
}

void _system::executeProgram( _program* prog , _cmdArgs args )
{
	_system::_programs_.push_back( make_pair( prog , args ) );
	prog->init( _system::_gadgetHost_ , args );
}

void _system::terminateProgram( _program* prog )
{
	// Erase the Program-Instance in the list of running instances
	_system::_programs_.remove_if( [&]( pair<_program*,_cmdArgs> s )->bool{ return s.first == prog; } );
	delete prog;
}

void _system::main()
{
	//_direntry d = _direntry("hallo.exe");
	//d.execute();
	SetYtrigger( 192 );
	irqEnable( IRQ_VCOUNT );
	int i = 0;
	/*_bitmap bmp = _bitmap( bgGetGfxPtr(_system::bgIdBack) , SCREEN_WIDTH , SCREEN_HEIGHT );
			cpuStartTiming(0);
			bmp.move( 0 , 0 , 10, 10 , 10 , 10 );
			bmp.move( 10 , 10 , 20, 20 , 10 , 10 );
			printf("%d\n",cpuGetTiming() );*/
	while(true)
	{
		// wait until line 0 
		//swiIntrWait( 192, IRQ_VCOUNT );
		
		//printf( "%d\n" , REG_VCOUNT );
		_system::processEvents();
		_system::runAnimations();
		_system::runPrograms();
		//consoleClear();
		/*if( _currentFocus_ )
			printf("cF: %s\n",gadgetType2string[_currentFocus_->getType()].c_str());
		for( _gadget* g : _gadgetHost_->children )
			printf("- %s, %d\n",gadgetType2string[g->getType()].c_str(),g->hasFocus() );*/
		//BG_PALETTE_SUB[0] = RGB( 20 , 20 , 20 );
		swiWaitForVBlank();
		/*//BG_PALETTE_SUB[0] = RGB( 31 , 31 , 31 );
		if( ++i > 120 )
		{
			const unsigned int FreeMemSeg=8*1024;
			
			unsigned int s;
			for( s = FreeMemSeg ; s < 4096 * 1024 ; s += FreeMemSeg )
			{
				void *ptr=new char[ s ];
				consoleClear();
				printf("free: %d\n",s-FreeMemSeg);
				//swiWaitForVBlank();
				delete ptr;
				ptr = 0;
			}
		}*/
	}
}

_program* _system::getBuiltInProgram( string qualifiedName ){
	/*if( qualifiedName == "explorer.exe" ){
		return new PROG_Explorer();
	}*/
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
list<_animation*>				_system::_animations_;
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