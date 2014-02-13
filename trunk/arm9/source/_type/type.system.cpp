#include "_type/type.system.h"
#include "_type/type.system.controller.h"
#include "_type/type.tokenizer.h"
#include "_type/type.binfile.h"
#include "_type/type.sound.h"
#include "_type/type.sound.stream.h"
#include "_type/type.time.h"
#include "_type/type.user.guest.h"
#include "_gadget/gadget.windows.h"
#include "func.memory.h"
#include <nds/timers.h>
#include <nds/arm9/video.h>
#include <nds/arm9/background.h>
#include <nds/arm9/console.h>
#include <nds/interrupts.h>
#include <dswifi9.h>
#include <fat.h>

/*void _system::restart(){
	struct __bootstub *bootcode = transfer.bootcode;
	bootcode->arm9reboot();
}*/

void _system::debug( const char* fmt , ... )
{
	if( !fmt )
		return;
	
	static char output[256];

    // Declare a va_list type variable
    va_list args;

    // Initialise the va_list variable with the ... after fmt
    va_start(args, fmt);

    // Forward the '...' to sprintf
    vsprintf(output, fmt , args);

    // Clean up the va_list
    va_end(args);
	
	
	// Enhance the message!
	string result = ( string( _time::now() ) + ": " + string( output ) + "\r\n" );
	
	// Debug to file
	_system::_debugFile_->writeString( result );
	
	// Debug to screen
	printf( "%s" , result.c_str() );
}

void _system::vdebug( const char* fmt , va_list args )
{
	if( !fmt )
		return;
	
	static char output[256];

    // Forward the '...' to sprintf
    vsprintf(output, fmt , args);
	
	// Enhance the message!
	string result = ( string( _time::now() ) + ": " + string( output ) + "\r\n" );
	
	// Debug to file
	_system::_debugFile_->writeString( result );
	
	// Debug to screen
	printf( "%s" , result.c_str() );
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

void _system::switchUser( _user&& usr )
{
	_system::_rtA_->setUser( move(usr) );
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

void _system::deleteGadgetHost()
{
	if( _system::_gadgetHost_ )
	{
		_system::enterCriticalSection();
		
		// Remove all programs that will be exeucted
		// the next frame beacause they all have the wrong gadgetHost Set
		_program::terminateAllPrograms();
		
		// Remove all events on this gadgetHost
		_system::removeEventsOf( _system::_gadgetHost_ );
		
		// Delete it
		delete _system::_gadgetHost_;
		
		_system::leaveCriticalSection();
		
		// Reset
		_system::_gadgetHost_ = nullptr;
	}
}

void _system::deleteKeyboard()
{
	if( _system::_keyboard_ )
	{		
		// Close Keyboard
		_system::_keyboard_->close( false );
		
		_system::enterCriticalSection();
		
		// Remove the keyboard!
		_system::removeEventsOf( _system::_keyboard_ );
		
		delete _system::_keyboard_;
		_system::_keyboard_ = nullptr;
		
		_system::leaveCriticalSection();
	}
}

void _system::vblHandler()
{
	if( _system::_gadgetHost_ || _system::_keyboard_ )
		_system::processInput();
	if( _system::_keyboard_ )
		_system::_keyboard_->vbl();
	
	// Animations have to be executed !during! the vertical Blank
	_animation::runAnimations();
}

//void _system::hblHandler()
//{
//	_system::runTimers();
//}

void _system::optimizeEvents()
{
	// Shortcut
	_eventList& events = _system::_eventBuffer_[!_system::_curEventBuffer_];
	
	sort( events.begin() , events.end() ,
		[]( _pair<_event,_eventCallType> e1 , _pair<_event,_eventCallType> e2)->bool{
			if( e1.first.getType() == e2.first.getType() )
			{
				if( e1.first != onDraw ) // Only sort events other than 'onDraw'
					return e1.first.getDestination() < e2.first.getDestination();
				else
					return false;
			}
			return e1.first.getType() < e2.first.getType();
		}
	);
	
	events.erase(
		unique( events.begin() , events.end() , []( _pair<_event,_eventCallType>& e1 , _pair<_event,_eventCallType>& e2 )->bool{ return e1.second == e2.second && e1.first.mergeWith( e2.first ); } )
		, events.end()
	);
}

void _system::processEvents()
{	
	// Do not throw any Events until we finished iterating through Events!!!!
	// -> This was a big Problem - Hours of finding that out!
	eventsSwapBuffer();
	
	// Optimize out unneccesary events
	optimizeEvents();
	
	for( _pair<_event,_eventCallType>& data : _system::_eventBuffer_[!_system::_curEventBuffer_] )
	{
		// Temp...
		_event& event = data.first;
		_gadget* gadget = event.getDestination();
		
		//int t = cpuGetTiming();
		
		// Make the Gadget ( if one is specified ) react on the event
		if( gadget != nullptr )
		{
			// Choose the right call-method
			switch( data.second )
			{
				case _eventCallType::normal:
					gadget->handleEvent( move(event) );
					break;
				case _eventCallType::normalNoDef:
					gadget->handleEvent( move(event) , true );
					break;
				case _eventCallType::user:
					gadget->handleEventUser( move(event) );
					break;
				case _eventCallType::internal:
					gadget->handleEventInternal( move(event) );
					break;
				case _eventCallType::def:
					gadget->handleEventDefault( move(event) );
					break;
				default:
					break;
			}
		}
		
		//printf("%d\n",cpuGetTiming()-t);
	}
	
	_system::_eventBuffer_[!_system::_curEventBuffer_].clear();
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
	const _user& user = _system::getUser();
	
	_u16 keys = keysHeld();
	
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
		if( _system::_keyboard_->isOpened() )
		{
			keys &= ~KEY_L;
			keys &= ~KEY_R;
			if( keysDown() & KEY_L )
			{
				if( _system::_keyboard_->isShift() )
					_system::_keyboard_->setShift( false );
				else
					_system::_keyboard_->setShift( true );
			}
			if( keysDown() & KEY_R )
			{
				if( _system::_keyboard_->isCaps() )
					_system::_keyboard_->setCaps( false );
				else
					_system::_keyboard_->setCaps( true );
			}
		}
	}
	
	if( keysDown() & KEY_START && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows ){
		_windows* win = (_windows*)_system::_gadgetHost_;
		win->toggleStartMenu();
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
		
		event.setKeyCode( _hardwareKeyPattern::libndsBit2key(i) );
		
		// held down
		if( GETBIT( keys , i ) )
		{
			if( heldCycles[i] == 0 )
			{
				if( _system::_currentFocus_ )
					_system::_currentFocus_->triggerEvent( event.setType( onKeyDown ) );
				else
					_system::_gadgetHost_->triggerEvent( event.setType( onKeyDown ) );
			}
			else if( user.kRD && heldCycles[i] > user.kRD && heldCycles[i] % user.kRS == 0 )
			{
				// Set the Args and Trigger the Event
				if( _system::_currentFocus_ )
					_system::_currentFocus_->triggerEvent( event.setType( onKeyRepeat ) );
				else
					_system::_gadgetHost_->triggerEvent( event.setType( onKeyRepeat ) );
			}
			
			// Increase Cycles
			heldCycles[i]++;
		}
		// Not held down and was pressed previously
		else if( heldCycles[i] > 0 )
		{
			// Set the Args
			event.setType( onKeyUp );
			
			// Trigger the Event
			if( _system::_currentFocus_ )
				_system::_currentFocus_->triggerEvent( event );
			else
				_system::_gadgetHost_->triggerEvent( event );
			
			
			// If keyup is fast enough, trigger keyClick
			if( heldCycles[i] < user.mCC )
			{
				if( _system::_currentFocus_ )
					_system::_currentFocus_->triggerEvent( event.setType( onKeyClick ) );
				else
					_system::_gadgetHost_->triggerEvent( event.setType( onKeyClick ) );
			}
			
			// Reset Cycles
			heldCycles[i] = 0;
		}
	}
}

void _system::start()
{
	//! Power on everything 
	powerOn( POWER_ALL );
	
	// ------------------------------------------------------------------------
	// Display 
	// ------------------------------------------------------------------------
		
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
		
		setBackdropColor( _color::black );
		setBackdropColorSub( _color::black );
		consoleDemoInit();
		
		//_system::fadeMainScreen( false , false );
		
		//memSet( BG_GFX_SUB , 0 , SCREEN_HEIGHT*SCREEN_WIDTH );
		//consoleInit	( nullptr , 1 , BgType_Text4bpp , BgSize_T_256x256 , 31 , 0 , true , true );
		
	// ------------------------------------------------------------------------
	// Interrupts
	// ------------------------------------------------------------------------
	
		//! Set the VBLANK Interrupt handler
		SetYtrigger(0); // Number of the scanline that generates a Vcount IRQ
		irqSet( IRQ_VBLANK , _system::vblHandler );
		//irqSet( IRQ_HBLANK , _system::hblHandler );
		irqEnable( IRQ_VBLANK | IRQ_VCOUNT );
		
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
	
	// ------------------------------------------------------------------------
	// System-Attributes
	// ------------------------------------------------------------------------
		
		//! init rand() generator
		srand( time(NULL) );
		
		//! enable Sound
		_sound::enable();
		
		//! Set Memory-Alloc-Error-Handler
		std::set_new_handler( &_system::newHandler );
	
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
	
		_system::_rtA_ = new _runtimeAttributes(nullptr);
	
	// -----------------------------------------------
	//	Open necesary Files
	// -----------------------------------------------
	
		_system::_debugFile_ = new _direntry("%WINDIR%/debug.txt");
		_system::_debugFile_->create();
		_system::_debugFile_->openwrite();
		
		_system::_registry_ = new _registry("%SYSTEM%/registry.reg");
		_system::_registry_->create();
	
	// -----------------------------------------------
	// Localization-System
	// -----------------------------------------------
	
		// Set Language
		_system::setLanguage( _system::_registry_->getLanguage() );
		
		// Localization of Strings
		_system::_localizationTextTable_ = new _ini( (string) _binFile( "%SYSTEM%/localizationText.ini" ) );
		
		// Localization of Months
		_system::_localizationMonthTable_ = new _ini( (string) _binFile( "%SYSTEM%/localizationMonth.ini" ) );
	
	// -----------------------------------------------
	// Add a temporary User
	// -----------------------------------------------
		
		_guestUser* guest = new _guestUser();
		guest->create();
		_system::_rtA_->setUser( move(guest) );
	
	// -----------------------------------------------
	// Gadget-System
	// -----------------------------------------------
		
		_system::_topScreen_ = new _screen( _bgIdSub_ );
		
	// -----------------------------------------------
	// Fonts
	// -----------------------------------------------
	
		_system::_fonts_["System7"]			= _font::fromFile( "%SYSTEM%/system7.ttf");
		_system::_fonts_["System10"]		= _font::fromFile( "%SYSTEM%/system10.ttf");
		_system::_fonts_["ArialBlack13"]	= _font::fromFile( "%SYSTEM%/arialblack13.ttf");
		_system::_fonts_["CourierNew10"]	= _font::fromFile( "%SYSTEM%/couriernew10.ttf");
		_system::_fonts_["SystemSymbols8"]	= _font::fromFile( "%SYSTEM%/systemsymbols8.ttf");
	
}

_language _system::getLanguage()
{
	static _map<string,_language> mp = {
		{ "e" , _language::english }
		,{ "f" , _language::french }
		,{ "g" , _language::german }
		,{ "i" , _language::italian }
		,{ "s" , _language::spanish }
	};
	
	return mp[_system::_curLanguageShortcut_];
}

const string& _system::getDSUserName()
{
	static string name;
	if( name.empty() && PersonalData->nameLen > 0 )
	{
		for( int i = 0 ; i < PersonalData->nameLen ; i++ )
			name += PersonalData->name[i];
	}
	return name;
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
	
	// Apply to registry
	_system::_registry_->setLanguage( lang );
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
	_systemController::main();
}

bool _system::isRunningOnEmulator()
{
	// Figure out what we're running on
	// http://forum.gbadev.org/viewtopic.php?t=6265
	_u32 result; 
	asm volatile("swi 0x0D\n" 
		"mov r0, %0\n" : "=r"(result) : : "r1", "r2", "r3"
	);
	return result != 0xBAAE1880 /* DS Mode */ && result != 0xBAAE187F;
}

bool _system::executeCommand( const string& cmd )
{
	if( cmd.empty() )
		return false;
	
	size_t fileNameEnd = string::npos;
	
	if( cmd[0] == '"' ) // If it starts with an apostroph
		fileNameEnd = cmd.find( '"' , 1 );
	else
		fileNameEnd = cmd.find( ' ' , 1 );
	
	return _direntry( cmd.substr( cmd[0] == '"' , fileNameEnd ) ).execute( fileNameEnd != string::npos ? cmd.c_str() + fileNameEnd : _programArgs() );
}

void _system::shutDown(){
	end();
}

//! Static Attributes...
bool 						_system::_sleeping_ = false;
_map<string,const _font*>	_system::_fonts_;
_ini*						_system::_localizationTextTable_;
_ini*						_system::_localizationMonthTable_;
string						_system::_curLanguageShortcut_;
_gadgetScreen*				_system::_gadgetHost_ = nullptr;
_screen*					_system::_topScreen_ = nullptr;
_keyboard*					_system::_keyboard_ = nullptr;
_registry*					_system::_registry_ = nullptr;
_runtimeAttributes*			_system::_rtA_;
const string				_system::_emptyStringSignature_ = "[]";
_direntry*					_system::_debugFile_ = nullptr;
_gadget*					_system::_currentFocus_ = nullptr;
_gadget*					_system::_lastClickedGadget_ = nullptr;
int							_system::_bgIdFront_;
int							_system::_bgIdBack_;
int							_system::_bgIdSub_;
int							_system::_cpuUsageTemp_;

//! Events
int							_system::_curEventBuffer_ = 0;
_eventList					_system::_eventBuffer_[2];