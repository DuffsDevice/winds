#include "_type/type.system.h"
#include "_type/type.system.controller.h"
#include "_type/type.binfile.h"
#include "_type/type.sound.h"
#include "_gadget/gadget.windows.h"
#include "func.memory.h"
#include <time.h>
#include <stdio.h>
#include <nds/timers.h>
#include <nds/arm9/video.h>
#include <nds/arm9/background.h>
#include <nds/arm9/console.h>
#include <nds/interrupts.h>
#include <dswifi9.h>
#include <fat.h>

#define transfer (*(__TransferRegion volatile *)(0x02FFF000))

struct __TransferRegion{
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

void _system::debug( string msg )
{
	// Enhance the message!
	msg = string( _time::now() ) + ": " + msg + "\r\n";
	
	// Debug to file
	_system::_debugFile_->writeString( msg );
	
	// Debug to screen
	printf( "%s" , msg.c_str() );
}

void _system::fadeMainScreen( bool out , bool anim )
{
	_tempTime time = _system::getHighResTime();
	
	REG_BLDCNT = ( 1 << 3 ) | ( 1 << 2 ) // 3rd and 2nd Screen_Layer
	| ( 3 << 6 ) ; // Set Blend Mode to fade to black ( 2 << 6 ) would be fading to white
	
	if( out )
	{
		while( anim && time + 200 > _system::getHighResTime() )
			REG_BLDY = 31 - float(_system::getHighResTime() - time )/(200)*31;
		
		REG_BLDY = 0;
	}
	else
	{
		while( anim && time + 200 > _system::getHighResTime() )
			REG_BLDY = float(_system::getHighResTime() - time )/(200)*31;
		
		REG_BLDY = 31;
	}
}

//! the speed in which the timer ticks in hertz.
//! #define BUS_CLOCK (33513982)
//! BUS_CLOCK >> 15 - BUS_CLOCK >> 21 - BUS_CLOCK >> 22 = 1000 * rawTime
//! There you get milliseconds from rawTime!

_tempTime _system::getHighResTime()
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
	
	_tempTime time = ( lo | mi<<16 | hi << 32);
	return ( time >> 15 ) - ( time >> 21 ) - ( time >> 22 ); // Equals time/1000
}

void _system::terminateTimer( const _callback& cb ){
	_timers_.remove_if(
		[&]( _pair<const _callback*,_callbackData> data )->bool{
			if( ( *data.first == cb ) == 1 )
			{
				delete data.first;
				return true;
			}
			return false;
		}
	);
}

void _system::runTimers()
{
	_tempTime curTime = getHighResTime();
	
	_timers_.remove_if( 
		[&]( _pair<const _callback*,_callbackData>& cb )->bool
		{
			_callbackData& data = cb.second;
			while( curTime > data.duration + data.startTime ) // "while" instead of "if" to make sure even high-frequency timers get called the right times
			{
				(*cb.first)();
				if( data.repeating )
					data.startTime += data.duration;
				else
					return true;
			}
			return false;
		}
	);
}

void _system::runPrograms()
{
	_programs_.remove_if(
		[]( pair<_program*,_cmdArgs>& prog )->bool{
			if( prog.first->autoDelete ){
				delete prog.first;
				return true;
			}
			return false;
		}
	);
}

void _system::deleteGadgetHost()
{
	if( _system::_gadgetHost_ )
	{
		irqDisable( IRQ_VBLANK ); // Enter critical Section
		
		// Remove all running programs on this gadgetHost
		_programs_.remove_if(
			[=]( pair<_program*,_cmdArgs>& prog )->bool{
				if( prog.first->getGadgetHost() == _system::_gadgetHost_ ){
					delete prog.first;
					return true;
				}
				return false;
			}
		);
		
		// Remove all events on this gadgetHost
		_system::removeEventsOf( _system::_gadgetHost_ );
		
		// Dlete it
		delete _system::_gadgetHost_;
		
		irqEnable( IRQ_VBLANK ); // Leave critical Section
		
		_system::_gadgetHost_ = nullptr;
	}
}

void _system::deleteKeyboard()
{
	if( _system::_keyboard_ )
	{		
		// Close Keyboard
		_system::_keyboard_->close( false );
		
		irqDisable( IRQ_VBLANK ); // Enter critical Section
		
		// Remove the keyboard!
		_system::removeEventsOf( _system::_keyboard_ );
		delete _system::_keyboard_;
		
		irqEnable( IRQ_VBLANK );  // Leave critical Section
		
		_system::_keyboard_ = nullptr;
	}
}

void _system::vblHandler()
{
	if( _system::_gadgetHost_ )
		_system::processInput();
	if( _system::_keyboard_ )
		_system::_keyboard_->screenVBL();
	_system::processEvents();
	_system::runAnimations();
	_system::runTimers();
	//_systemController::controllerVBL(); // is called in _systemController::main
}

/*void optimizeEvents()
{
	//! Optimize refresh-Events
	map<_gadget*,_event*);
	deque<_event> tempEvents = this->events;
	
	sort( tempEvents.begin() , tempEvents.end() , [](_event e1 , _event e2)->bool{ return ( e1.getDestination() < e2.getDestination() ); } );
	
	_gadget* dest = 0;
	_gadget* tempDest = 0;
	_area refresh,damaged;
	refresh.clear();
	damaged.clear();
	
	this->events.clear();
	
	for( auto it = tempEvents.begin() ; it != tempEvents.end() ; it++ )
	{
		tempDest = (_gadget*) it->getDestination();
		
		damaged.push_back( it->getRefreshRects() );
		damaged.push_back( it->getDamagedRects() );
		
		if( dest != 0 && dest != tempDest ){
			it->setDamagedRects( damaged );
			it->setRefreshRects( refresh );
			this->events.push_back( *it );
		}
		dest = tempDest;
	}
	
	this->events = tempEvents;
}*/

void _system::processEvents()
{
	//optimizeEvents();
	
	// Do not throw any Events until we finished iterating through Events!!!!
	// -> This was a big Problem - Hours of finding that out!
	eventsSwapBuffer();
	
	for( _event& event : _system::_eventBuffer_[!_system::_curEventBuffer_] )
	{
		// Temp...
		_gadget* gadget = event.getDestination();
		
		//int t = cpuGetTiming();
		
		// Make the Gadget ( if one is specified ) react on the event
		if( gadget != nullptr )
			gadget->handleEvent( (_event&&)event );
		
		//printf("%d\n",cpuGetTiming()-t);
	}
	
	_system::_eventBuffer_[!_system::_curEventBuffer_].clear();

	//if( !_system::_eventBuffer_[_system::_curEventBuffer_].size() )
	//{
	//	j++;
	//	addMethod( reinterpret_cast<void*>(&_memoryfont::drawCharacter),"fontDrawing");
	//	addMethod( reinterpret_cast<void*>(&_gadget::gadgetRefreshHandler),"refresh");
	//	addMethod( reinterpret_cast<void*>(&_bitmap::copyTransparent),"copyAlgo");
	//	addMethod( reinterpret_cast<void*>(&_rect::reduce),"reduceAlgo");
	//	addMethod( reinterpret_cast<void*>(&_rect::toRelative),"toRelative");
	//	
	//	printResults();
	//	if( j > 60 )
	//	{
	//		printf("%d\n",z);
	//		while(true);
	//	}
	//}
}


void _system::processInput()
{
	// Refresh Input
	scanKeys();
	
	touchPosition t;
	touchRead( &t );

	// Temp...
	static _u32 heldCycles[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	_event event = _event().setCurrentKeys( _system::getCurrentKeys() );
	
	// Shortcut...
	const _user& user = *_system::getUser();
	
	_key keys = keysHeld();
	
	if( !_system::_keyboard_ || !_system::_keyboard_->processTouch( keys & KEY_TOUCH , t ) )
		_system::_gadgetHost_->processTouch( keys & KEY_TOUCH , t );
	
	if( _system::_keyboard_ )
	{
		if( keysDown() & KEY_SELECT )
		{
			if( _system::_keyboard_->isOpened() )
				_system::_keyboard_->close();
			else
				_system::_keyboard_->open();
		}
		if( keysDown() & KEY_L )
		{
			if( _system::_keyboard_->isOpened() )
			{
				if( _system::_keyboard_->isShift() )
					_system::_keyboard_->setShift( false );
				else
					_system::_keyboard_->setShift( true );
			}
		}
		if( keysDown() & KEY_R )
		{
			if( _system::_keyboard_->isOpened() )
			{
				if( _system::_keyboard_->isCaps() )
					_system::_keyboard_->setCaps( false );
				else
					_system::_keyboard_->setCaps( true );
			}
		}
	}
	
	if( keysDown() & KEY_START && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
	{
		_windows* win = (_windows*)_system::_gadgetHost_;
		
		if( win->isStartMenuOpened() )
			win->closeStartMenu();
		else
			win->openStartMenu();
	}

	if( !_system::_currentFocus_ && !_system::_gadgetHost_ )
		return;
	
	/**
	 * Just Handle the Buttons, not the Pen!
	 */
	for( _u8 i = 0 ; i < 16 ; ++i )
	{
		//! Again: We do not handle Pen (as well as the lid)
		if( BIT(i) == KEY_TOUCH || BIT(i) == KEY_LID ) continue;
		
		event.setKeyCode( DSWindows::libnds2key[i] );
		
		//printf("CF: %s\n", gadgetType2string[_system::_currentFocus_->getType() ].c_str() );
		
		// held down
		if( GETBIT( keys , i ) )
		{
			if( heldCycles[i] == 0 )
			{
				if( _system::_currentFocus_ )
					_system::_currentFocus_->handleEvent( event.setType( keyDown ) );
				else
					_system::_gadgetHost_->handleEvent( event.setType( keyDown ) );
			}
			else if( user.kRD && heldCycles[i] > user.kRD && heldCycles[i] % user.kRS == 0 )
			{
				// Set the Args and Trigger the Event
				if( _system::_currentFocus_ )
					_system::_currentFocus_->handleEvent( event.setType( keyRepeat ) );
				else
					_system::_gadgetHost_->handleEvent( event.setType( keyRepeat ) );
			}
			
			// Increase Cycles
			heldCycles[i]++;
		}
		// Not held down and was pressed previously
		else if( heldCycles[i] > 0 )
		{
			// Set the Args
			event.setType( keyUp );
			
			// Trigger the Event
			if( _system::_currentFocus_ )
				_system::_currentFocus_->handleEvent( event );
			else
				_system::_gadgetHost_->handleEvent( event );
			
			
			// If keyup is fast enough, trigger keyClick
			if( heldCycles[i] < user.mCC )
			{
				if( _system::_currentFocus_ )
					_system::_currentFocus_->handleEvent( event.setType( keyClick ) );
				else
					_system::_gadgetHost_->handleEvent( event.setType( keyClick ) );
			}
			
			// Reset Cycles
			heldCycles[i] = 0;
		}
	}
}

void _system::start()
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
		_system::_bgIdFront_ = bgInit(2, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
		_system::_bgIdBack_ = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 8, 0);
		_system::_bgIdSub_ = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
		
		setBackdropColor( COLOR_BLACK );
		setBackdropColorSub( COLOR_BLACK );
		consoleDemoInit();
		
		//_system::fadeMainScreen( false , false );
		
		//memSet( BG_GFX_SUB , 0 , SCREEN_HEIGHT*SCREEN_WIDTH );
		//consoleInit	( nullptr , 1 , BgType_Text4bpp , BgSize_T_256x256 , 31 , 0 , true , true );
		
	// ------------------------------------------------------------------------
	// Interrupts
	// ------------------------------------------------------------------------
	
		//! Set the VBLANK Interrupt handler
		SetYtrigger( 0 );
		irqSet( IRQ_VBLANK , _system::vblHandler );
		//irqSet( IRQ_VCOUNT , _system::_vcount_ );
		irqEnable( IRQ_VCOUNT | IRQ_VBLANK );
		
		//! Start Timers
		TIMER_CR(0) = 0;
		TIMER_CR(1) = 0;
		TIMER_CR(2) = 0;

		TIMER_DATA(0) = 0;
		TIMER_DATA(1) = 0;
		TIMER_DATA(2) = 0;

		TIMER_CR(2) = TIMER_CASCADE | TIMER_ENABLE;
		TIMER_CR(1) = TIMER_CASCADE | TIMER_ENABLE;
		TIMER_CR(0) = TIMER_ENABLE;
		
		
		/*_bitmap bmp = _bitmap( BG_GFX , SCREEN_WIDTH , SCREEN_HEIGHT );
		
		_area b = { _rect( 40 , 40 , 40 , 40 ) , _rect( 50 , 50 , 40 , 40 ) };
		for( _rect rc : b )
			bmp.drawRect( rc.x , rc.y , rc.width , rc.height , RGB( 0 , 0 , 31 ) );
			
		_area a = _rect( 40 , 40 , 40 , 40 ).combine(_rect( 40 , 40 , 50 , 40 ));
		
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
		
		//! init rand() generator
		srand( time(NULL) );
		
		//! enable Sound
		_sound::enable();
		
		//! Set Memory-Alloc-Error-Handler
		set_new_handler( &_system::newHandler );
		
		_system::setLanguage( (_language) PersonalData->language );
	
	// -----------------------------------------------
	//	Hardware Init
	// -----------------------------------------------
	
		bool fatInit = _direntry::initFat();
		bool wifiInit = Wifi_InitDefault(INIT_ONLY);
		
		if( fatInit && !wifiInit )
			_system::debug("Wifi could not be inited! Please note that the default Firmware Settings are used to connect");
	
	// -----------------------------------------------
	//	RTA - Runtime Attributes
	// -----------------------------------------------
	
		_system::_rtA_ = new _runtimeAttributes( new _user("Guest") );
	
	// -----------------------------------------------
	//	Open necesary Files
	// -----------------------------------------------
	
		_system::_debugFile_ = new _direntry("%WINDIR%/debug.txt");
		_system::_debugFile_->create();
		
		_system::_registry_ = new _registry("%WINDIR%/windows.reg");
		
		// Localization of Strings
		_system::_localizationTextTable_ = new _registry( (string) _binfile( "%SYSTEM%/localizationText.ini" ) );
		
		// Localization of Months
		_system::_localizationMonthTable_ = new _registry( (string) _binfile( "%SYSTEM%/localizationMonth.ini" ) );
	
	// -----------------------------------------------
	// Gadget-System
	// -----------------------------------------------
		
		_system::_topScreen_ = new _screen( _bgIdSub_ );
		
	// -----------------------------------------------
	// Fonts
	// -----------------------------------------------
	
		_system::_fonts_["ArialBlack13"]	= _font::fromFile( "%SYSTEM%/arialblack13.ttf");
		_system::_fonts_["CourierNew10"]	= _font::fromFile( "%SYSTEM%/couriernew10.ttf");
		_system::_fonts_["System7"]			= _font::fromFile( "%SYSTEM%/system7.ttf");
		_system::_fonts_["SystemSymbols8"]	= _font::fromFile( "%SYSTEM%/systemsymbols8.ttf");
}

_language _system::getLanguage()
{
	static map<string,_language> mp = {
		{ "e" , _language::english }
		,{ "f" , _language::french }
		,{ "g" , _language::german }
		,{ "i" , _language::italian }
		,{ "s" , _language::spanish }
	};
	
	return mp[_system::_curLanguageShortcut_];
}

void _system::setLanguage( _language lang )
{
	switch( lang )
	{
		case _language::japanese: // Japanese
		case _language::english: // English
		case _language::chinese: // Chinese
			_system::_curLanguageShortcut_ = "e";
			break;
		case _language::french: // French
			_system::_curLanguageShortcut_ = "f";
			break;
		case _language::german: // German
			_system::_curLanguageShortcut_ = "g";
			break;
		case _language::italian: // Italian
			_system::_curLanguageShortcut_ = "i";
			break;
		case _language::spanish: // Spanish
			_system::_curLanguageShortcut_ = "s";
			break;
		default:
			_system::_curLanguageShortcut_ = "e";
	}
}

void _system::newHandler()
{
	_system::debug("Out of memory!");
	while(true)
		swiWaitForVBlank();
}

void _system::end()
{
	delete _system::_registry_;
	delete _system::_debugFile_;
	
	_system::deleteGadgetHost();
	_system::deleteKeyboard();
	
	systemShutDown();
}

void _system::submit(){
	while( !(keysDown() & KEY_A) )
		scanKeys();
	while( keysDown() & KEY_A )
		scanKeys();
}

void _system::main(){
	//printf("Size: %d\n",sizeof(Loki::AssocVector<_eventType,_callback*>));
	_systemController::main();
}

bool _system::executeCommand( string cmd ){
	//if( qualifiedName == "explorer.exe" ){
	//	return new PROG_Explorer();
	//}
	//return nullptr;
}

void _system::shutDown(){
	end();
}

//! Static Attributes...
bool 							_system::_sleeping_ = false;
_list<_animation*>				_system::_animations_;
_list<_pair<const _callback*,
		_callbackData>>			_system::_timers_;
_map<string,const _font*>		_system::_fonts_;
_registry*						_system::_localizationTextTable_;
_registry*						_system::_localizationMonthTable_;
string							_system::_curLanguageShortcut_;
_list<_pair<_program*,_cmdArgs>>_system::_programs_;
_gadgetScreen*					_system::_gadgetHost_ = nullptr;
_screen*						_system::_topScreen_ = nullptr;
_keyboard*						_system::_keyboard_ = nullptr;
_registry*						_system::_registry_ = nullptr;
_runtimeAttributes*				_system::_rtA_;
_direntry*						_system::_debugFile_ = nullptr;
_gadget*						_system::_currentFocus_ = nullptr;
_gadget*						_system::_lastClickedGadget_ = nullptr;
int								_system::_bgIdFront_;
int								_system::_bgIdBack_;
int								_system::_bgIdSub_;
int								_system::_cpuUsageTemp_;

//! Events
int								_system::_curEventBuffer_ = 0;
_list<_event> 					_system::_eventBuffer_[2];