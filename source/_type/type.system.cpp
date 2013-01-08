#include "_type/type.system.h"
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

#include "strings_bin.h"

//! Resources we need
#include "_resource/PROG_Explorer.h"
#include "_resource/BMP_Checkboxes.h"
#include "_resource/FONT_ArialBlack10.h"
#include "_resource/FONT_CourierNew10.h"
#include "_resource/FONT_Tahoma7.h"

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

void _system::debug( string msg ){
	time_t rawtime = time(NULL);
	struct tm* t = localtime( &rawtime );
	_system::_debugFile_->writeString( asctime( t ) + msg + "\r\n" );
	printf( "%s" , (asctime( t ) + msg + "\n").c_str() );
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

_tempTime _system::getHighResTime(){
	_tempTime tmp = cpuGetTiming();
	return ( tmp >> 15 ) - ( tmp >> 21 ) - ( tmp >> 22 );
}

void _system::executeTimer( const _callback* cb , _tempTime duration , bool isRepeating )
{
	_timers_.push_back(
		make_pair(
			cb , 
			_callbackData( { getHighResTime() , duration , isRepeating } )
		)
	);
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
		irqDisable( IRQ_VBLANK );
		_system::removeEventsOf( _system::_gadgetHost_ );
		delete _system::_gadgetHost_;
		irqEnable( IRQ_VBLANK );
		
		_system::_gadgetHost_ = nullptr;
	}
}

void _system::deleteKeyboard()
{
	if( _system::_keyboard_ )
	{
		irqDisable( IRQ_VBLANK );
		_system::removeEventsOf( _system::_keyboard_ );
		delete _system::_keyboard_;
		irqEnable( IRQ_VBLANK );
		
		_system::_keyboard_ = nullptr;
	}
}

void _system::vblHandler()
{
	if( _system::_gadgetHost_ )
		_system::processInput();
	//if( _system::_keyboard_ )
	//	_system::_keyboard_->screenVBL();
	_system::processEvents();
	_system::runAnimations();
	_system::runTimers();
}

//static int z = 0;

void _system::processEvents()
{
	//optimizeEvents();
	// Do not throw any Events until we finished iterating through Events!!!!
	// -> This was a big Problem - Hours of finding that out!
	eventsSwapBuffer();
	
	// Temp...
	_gadget* gadget;
	
	for( _event& event : _system::_eventBuffer_[!_system::_curEventBuffer_] )
	{
		gadget = (_gadget*)event.getDestination();
		
		//int t = cpuGetTiming();
		
		// Make the Gadget ( if one is specified ) react on the event
		if( gadget != nullptr )
			gadget->handleEvent( event );
		//printf("%d\n",cpuGetTiming()-t);
	}

	// Erase all Events
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
	static _u16 lastKeys = 0; // 0 = No Keys
	static _u32 heldCycles[16] = {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	
	// Shortcut...
	const _user* user = _system::_runtimeAttributes_->user;
	
	_key keys = keysHeld();
	
	if( !_system::_keyboard_ || !_system::_keyboard_->processTouch( keys & KEY_TOUCH , t ) )
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
				// Set the Args and Trigger the Event
				_system::_currentFocus_->handleEvent( _event( keyDown ).setKeyCode( libnds2key[i] ).setCurrentKeyCodes( lastKeys ) );
			}
			
			// Increase Cycles
			heldCycles[i]++;
		}
		// Erase Cycles (but only if the Button of the Bit was pressed previously!)
		else if( heldCycles[i] > 0 )
		{
			// Set the Args
			_event event = _event( keyUp ).setKeyCode( libnds2key[i] ).setCurrentKeyCodes( lastKeys );
			
			// Trigger the Event
			_system::_currentFocus_->handleEvent( event );
			
			
			// If keyup is fast enough, trigger keyClick (only if the "button" wasn't the mouse
			if( heldCycles[i] < user->mCC )
				_system::_currentFocus_->handleEvent( event.setType( keyClick ) );
			
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
		
		//! Start Time
		cpuStartTiming(1);
		
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
		
		//! random Random() generator
		srand( time(NULL) );
		
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
	
		_system::_runtimeAttributes_ = new _runtimeAttributes;
		_system::_runtimeAttributes_->user = new _user("Guest");
	
	// -----------------------------------------------
	//	Open necesary Files
	// -----------------------------------------------
	
		_system::_debugFile_ = new _direntry("%WINDIR%/debug.txt");
		_system::_debugFile_->create();
		
		_system::_registry_ = new _registry("%WINDIR%/windows.reg");
		
		// Localization of Strings
		string str = (const char*)strings_bin;
		str.resize( strings_bin_size );
		_system::_localizationTable_ = new _ini( str );
		_system::_localizationTable_->read();
	
	// -----------------------------------------------
	// Gadget-System
	// -----------------------------------------------
		
		_system::_topScreen_ = new _screen( _bgIdSub_ );
		
	// -----------------------------------------------
	// Fonts
	// -----------------------------------------------
	
		_system::_fonts_["ArialBlack10"] = new FONT_ArialBlack10();
		_system::_fonts_["CourierNew10"] = new FONT_CourierNew10();
		_system::_fonts_["Tahoma7"] = new FONT_Tahoma7();
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
	//_system::blueScreen(121,"Not enough Memory!");
}

void _system::end()
{
	delete _system::_registry_;
	delete _system::_debugFile_;
	delete _system::_keyboard_;
	delete _system::_gadgetHost_;
	delete _system::_runtimeAttributes_->user;
	delete _system::_runtimeAttributes_;
	systemShutDown();
}

void _system::submit(){
	while( !(keysDown() & KEY_A) )
		scanKeys();
	while( keysDown() & KEY_A )
		scanKeys();
}

void _system::main()
{
	//_bitmap _screen_ = _bitmap( BG_GFX , 256 , 192 );
	//_screen_.drawHorizontalLine( 1 , 16 , 8 , COLOR_RED );
	//_screen_.drawVerticalLine( 16 , 1 , 8 , COLOR_RED );
	//_screen_.setClippingRect( _rect::fromCoords( 0 , 0 , 14 , 14 ) );
	//_screen_.fill( COLOR_WHITE );
	//_screen_.drawLine( 1 , 1 , 8 , 8 , COLOR_RED );
	_systemController::main();
	
	while(true)
	{
		swiWaitForVBlank();
		//consoleClear();
		//if( _currentFocus_ )
			//printf("cF: %s\n",gadgetType2string[_currentFocus_->getType()].c_str());
		/*for( _gadget* g : _gadgetHost_->children )
			printf("- %s, %d\n",gadgetType2string[g->getType()].c_str(),g->hasFocus() );*/
		/*if( ++i > 120 )
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
	if( qualifiedName == "explorer.exe" ){
		return new PROG_Explorer();
	}
	return nullptr;
}

void _system::shutDown(){
	end();
}

const string& _system::getLocalizedString( string name )
{
	auto& tmp = _system::_localizationTable_->getMap();
	if( !tmp.count( name ) || !tmp[name].count( _system::_curLanguageShortcut_ ) )
		return "[]";
	return _system::_localizationTable_->getMap()[name][_system::_curLanguageShortcut_];
}


//! Static Attributes...
bool 							_system::_sleeping_ = false;
_list<_animation*>				_system::_animations_;
_list<_pair<const _callback*,
		_callbackData>>			_system::_timers_;
_map<string,_font*>				_system::_fonts_;
_ini*							_system::_localizationTable_;
string							_system::_curLanguageShortcut_;
_list<_pair<_program*,_cmdArgs>>_system::_programs_;
_gadgetScreen*					_system::_gadgetHost_ = nullptr;
_screen*						_system::_topScreen_ = nullptr;
_keyboard*						_system::_keyboard_ = nullptr;
_registry*						_system::_registry_ = nullptr;
_runtimeAttributes*				_system::_runtimeAttributes_ = nullptr;
_direntry*						_system::_debugFile_ = nullptr;
_gadget*						_system::_currentFocus_ = nullptr;
int								_system::_bgIdFront_;
int								_system::_bgIdBack_;
int								_system::_bgIdSub_;
int								_system::_cpuUsageTemp_;

//! Events
int								_system::_curEventBuffer_ = 0;
_list<_event> 					_system::_eventBuffer_[2];