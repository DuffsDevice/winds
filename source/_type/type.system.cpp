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
#include "_resource/FONT_ArialBlack10.h"
#include "_resource/FONT_CourierNew10.h"
#include "_resource/FONT_Tahoma7.h"

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

_gadgetEventReturnType setupHandler( _gadgetEvent e )
{
	static int state = 0;
	
	
	_gadget* that = e.getGadget();
	
	if( typeOfGadget( that ) == _gadgetType::button )
	{
		if( that->getStyle().data == -1 )
		{
			if( state > 0 )
			{
				state--;
				that->getParent()->triggerEvent( _gadgetEvent( "setup" ) );
			}
		}
		else
		{
			if( state < 1 )
			{
				state++;
				that->getParent()->triggerEvent( _gadgetEvent( "setup" ) );
			}
		}
		return handled;
	}
	
	that->removeChildren( true );
	
	switch( that->getStyle().data )
	{
		case 0:{
			_label* lbl = new _label( 14 , 29 , "Welcome" );
			lbl->setColor( RGB( 2 , 5 , 15 ) );
			lbl->setFont( _system_->getFont( "ArialBlack10" ) );
			that->addChild( lbl );
			_label* lbl2 = new _label( 13 , 28 , "Welcome" );
			lbl2->setColor( RGB( 30 , 30 , 30 ) );
			lbl2->setFont( _system_->getFont( "ArialBlack10" ) );
			that->addChild( lbl2 );
			break;}
		case 1:
			that->addChild( new _label( 13 , 13 , "Second Setup Page" ) );
			break;
	}
	return handled;
}
void _system::setup()
{	
	// Clean up
	_system::deleteGadgetHost();
	//_system::deleteKeyboard();
	
	_system::_gadgetHost_ = new _startupScreen( bgIdBack , _gadgetStyle::storeData( 0 ) );
	_system::_keyboard_ = new _keyboard( bgIdFront , _system::_gadgetHost_ , _system::_topScreen_ );
	
	_system::_gadgetHost_->registerEventHandler( "setup" , setupHandler );
	_system::_gadgetHost_->triggerEvent( _gadgetEvent( "setup" ) );
	
	while( true )
		swiWaitForVBlank();
}

void _system::loginPage()
{
	// Clean up
	_system::deleteGadgetHost();
	
	_system::_gadgetHost_ = new _startupScreen( bgIdBack );
	_system::_keyboard_ = new _keyboard( bgIdFront , _system::_gadgetHost_ , _system::_topScreen_ );
}

void _system::initializeComponents()
{
	// Clean up
	_system::deleteGadgetHost();
	
	_u32 time = _system::getTime();
	
	// -----------------------------------------------
	// Create BootLogo
	// -----------------------------------------------
		
		_bootupScreen* screen;
		_system::_gadgetHost_ = screen = new _bootupScreen( bgIdBack );
		
	
	// End!
	while( _system::getTime() - time < 2000 )
		swiWaitForVBlank();
}

void _system::removeEventsOf( _gadget* g )
{
	// Delete events
	_system::eventBuffer[_system::curEventBuffer].remove_if( [&]( _gadgetEvent event )->bool{ return event.getDestination() == g; } );
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

_font* _system::getFont( string font )
{
	if( font.empty() || !_system::_fonts_.count( font ) )
		return _system::_fonts_[ _system::_runtimeAttributes_->defaultFont ];
	
	return _system::_fonts_[font];
}

_font* _system::getFont()
{
	return _system::_fonts_[ _system::_runtimeAttributes_->defaultFont ];
}

void _system::_vblank_()
{
	if( _system::_gadgetHost_ )
		_system::processInput();
	//if( _system::_keyboard_ )
	//	_system::_keyboard_->screenVBL();
	_system::processEvents();
	_system::runAnimations();
}

void _system::generateEvent( _gadgetEvent event )
{
	_system::eventBuffer[_system::curEventBuffer].push_back( event );
}

void _system::processEvents()
{
	//optimizeEvents();
	// Do not throw any Events until we finished iterating through Events!!!!
	// -> This was a big Problem - Hours of finding that out!
	eventsSwapBuffer();
	
	// Temp...
	_gadget* gadget;
	
	for( _gadgetEvent& event : _system::eventBuffer[!_system::curEventBuffer] )
	{
		gadget = (_gadget*)event.getDestination();
		
		//int s = cpuGetTiming();
		
		// Make the Gadget ( if one is specified ) react on the event
		if( gadget != nullptr )
			gadget->handleEvent( event );
		//printf("\n%d",cpuGetTiming()-s);
	}

	// Erase all Events
	_system::eventBuffer[!_system::curEventBuffer].clear();
	
	//if( !_system::eventBuffer[_system::curEventBuffer].size() )
	//{
	//	addMethod( reinterpret_cast<void*>(&_memoryfont::drawCharacter),"fontDrawing");
	//	addMethod( reinterpret_cast<void*>(&_gadget::gadgetRefreshHandler),"refresh");
	//	addMethod( reinterpret_cast<void*>(&_bitmap::copyTransparent),"copyAlgo");
	//	addMethod( reinterpret_cast<void*>(&_rect::reduce),"reduceAlgo");
	//	addMethod( reinterpret_cast<void*>(&_rect::toRelative),"toRelative");
	//	
	//	printResults();
	//	printf("%d\n",cpuGetTiming());
	//	while(true);
	//}
}

void _system::eventsSwapBuffer()
{
	_system::curEventBuffer = !_system::curEventBuffer;
}

_u16 _system::getCurrentKeys()
{
	return keysHeld() & (~(KEY_TOUCH|KEY_LID));
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
	const _user* user = _system_->_runtimeAttributes_->user;
	
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
				_system::_currentFocus_->handleEvent( _gadgetEvent( "keyDown" ).setKeyCode( libnds2key[i] ).setCurrentKeyCodes( lastKeys ) );
			}
			
			// Increase Cycles
			heldCycles[i]++;
		}
		// Erase Cycles (but only if the Button of the Bit was pressed previously!)
		else if( heldCycles[i] > 0 )
		{
			// Set the Args
			_gadgetEvent event = _gadgetEvent( "keyUp" ).setKeyCode( libnds2key[i] ).setCurrentKeyCodes( lastKeys );
			
			// Trigger the Event
			_system::_currentFocus_->handleEvent( event );
			
			
			// If keyup is fast enough, trigger keyClick (only if the "button" wasn't the mouse
			if( heldCycles[i] < user->mCC )
				_system::_currentFocus_->handleEvent( event.setType( "keyClick" ) );
			
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
		
		setBackdropColor( COLOR_BLACK );
		setBackdropColorSub( COLOR_BLACK );
		consoleDemoInit();
		
		//memSet( BG_GFX_SUB , 0 , SCREEN_HEIGHT*SCREEN_WIDTH );
		//consoleInit	( nullptr , 1 , BgType_Text4bpp , BgSize_T_256x256 , 31 , 0 , true , true );
		
	// ------------------------------------------------------------------------
	// Interrupts
	// ------------------------------------------------------------------------
	
		//! Set the VBLANK Interrupt handler
		SetYtrigger( 0 );
		irqSet( IRQ_VBLANK , _system::_vblank_ );
		//irqSet( IRQ_VCOUNT , _system::_vcount_ );
		irqEnable( IRQ_VCOUNT | IRQ_VBLANK );
		
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
		
		//! random Random() generator
		srand( time(NULL) );
		
		//! Set Memory-Alloc-Error-Handler
		set_new_handler( &_system::_newErrorFunc_ );
	
	// -----------------------------------------------
	//	Hardware Init
	// -----------------------------------------------
	
		bool fatInit = _direntry::initFat();
		bool wifiInit = Wifi_InitDefault(INIT_ONLY);
		
		if( fatInit && !wifiInit )
			_system::debug("Wifi could not be inited! Please note that the default Firmware Settings are used to connect");
	
	// -----------------------------------------------
	//	Open necesary Files
	// -----------------------------------------------
	
		_system::_debugFile_ = new _direntry("%WINDIR%/debug.txt");
		_system::_debugFile_->create();
		
		_system::_registry_ = new _registry("%WINDIR%/windows.reg");
	
	// -----------------------------------------------
	// Gadget-System
	// -----------------------------------------------
		
		_system::_topScreen_ = new _screen( bgIdSub );
		
	// -----------------------------------------------
	//	RTA - Runtime Attributes
	// -----------------------------------------------
	
		_system::_runtimeAttributes_ = new _runtimeAttributes;
		_system::_runtimeAttributes_->user = new _user("Jakob");
		
	// -----------------------------------------------
	// Fonts
	// -----------------------------------------------
	
		_system::_fonts_["ArialBlack10"] = new FONT_ArialBlack10();
		_system::_fonts_["CourierNew10"] = new FONT_CourierNew10();
		_system::_fonts_["Tahoma7"] = new FONT_Tahoma7();
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

_u32 _system::getCpuUsage()
{
	return _system::cpuUsageTemp;
}

void _system::terminateProgram( _program* prog )
{
	// Erase the Program-Instance in the list of running instances
	_system::_programs_.remove_if( [&]( pair<_program*,_cmdArgs> s )->bool{ return s.first == prog; } );
	delete prog;
}

void _system::main()
{
	// Fist of all, the Boot up
	//initializeComponents();
	
	// After the Boot up finishes,
	
	// see if this is a new Setup
	if( _system::_registry_->readIndex( "_global_" , "firstTimeUse" ).length() )
		setup();
	//_bitmap* bmp = new _bitmap( BG_GFX , SCREEN_WIDTH , SCREEN_HEIGHT );
	//bmp->drawString( 20 , 20 , _system::_runtimeAttributes_->defaultFont , "Hello World!" , COLOR_WHITE );
	
	
	//_direntry d = _direntry("hallo.exe");
	//d.execute();
	//int i = 0;
	/*_bitmap bmp = _bitmap( bgGetGfxPtr(_system::bgIdBack) , SCREEN_WIDTH , SCREEN_HEIGHT );
			cpuStartTiming(0);
			bmp.move( 0 , 0 , 10, 10 , 10 , 10 );
			bmp.move( 10 , 10 , 20, 20 , 10 , 10 );
			printf("%d\n",cpuGetTiming() );*/
	//int t = 0;
	while(true)
	{
		//printf( "%d\n" , cpuUsageTemp );
		_system::runPrograms();
		//consoleClear();
		//if( _currentFocus_ )
			//printf("cF: %s\n",gadgetType2string[_currentFocus_->getType()].c_str());
		/*for( _gadget* g : _gadgetHost_->children )
			printf("- %s, %d\n",gadgetType2string[g->getType()].c_str(),g->hasFocus() );*/
		//BG_PALETTE_SUB[0] = RGB( 20 , 20 , 20 );
		_system::cpuUsageTemp = ( _system::cpuUsageTemp + _system::cpuUsageTemp + REG_VCOUNT - 192 ) / 3;
		
		// wait until line 0 
		swiIntrWait( 0, IRQ_VCOUNT );
		
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
_list<_animation*>				_system::_animations_;
_map<string,_font*>				_system::_fonts_;
_list<pair<_program*,_cmdArgs>>	_system::_programs_;
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
int								_system::cpuUsageTemp;

//! Events
int								_system::curEventBuffer = 0;
_list<_gadgetEvent> 				_system::eventBuffer[2];

// Static system...
_system* _system_ = nullptr;