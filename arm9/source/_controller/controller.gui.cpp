#include "_controller/controller.gui.h"
#include "_controller/controller.interrupt.h"
#include "_controller/controller.registry.h"
#include "_screen/screen.keyboard.h"

// NDS Interrupts
#include <nds/interrupts.h>
#include <nds/bios.h>
#include <nds/system.h>

// Gui States
#include "_gui/gui.setup.h"
#include "_gui/gui.bootup.h"
#include "_gui/gui.desktop.h"
#include "_gui/gui.login.h"

// Graphic
#include "_resource/resource.image.window.header.h"

bool _guiController::init()
{
	top = new _screen( _videoController::getBgIdTop() );
	
	windowBar = BMP_WindowHeader();
	windowBarBlurred = BMP_WindowHeaderBlurred();
	
	//!@todo load from rgistry
	focusFg		= _color::white;
	focusBg		= _color::fromRGB8( 49 , 106 , 197 );
	selectFg	= _color::black;
	selectBg	= _color::fromRGB( 22 , 22 , 20 );
	itemFg		= _color::black;
	itemBg		= _color::transparent;
	controlBg	= _color::fromRGB( 28 , 28 , 27 );
	controlFg	= _color::fromRGB( 22 , 22 , 20 );
	
	// Load Some Attributes
	const _ini& registry = _registryController::getSystemRegistry();
	minDragDistance			= registry.readIndexInt( "gui" , "minDragDistance" );
	maxClickPeriod			= registry.readIndexInt( "gui" , "maxClickPeriod" );
	keyRepetitionDelay		= registry.readIndexInt( "gui" , "keyRepetitionDelay" );
	keyRepetitionPause		= registry.readIndexInt( "gui" , "keyRepetitionPause" );
	maxDoubleClickPeriod	= registry.readIndexInt( "gui" , "maxDoubleClickPeriod" );
	maxDoubleClickArea		= registry.readIndexInt( "gui" , "maxDoubleClickArea" );
	rightClickDelay			= registry.readIndexInt( "gui" , "rightClickDelay" );
	
	fileObjectHeight		= registry.readIndexInt( "gui" , "fileObjectHeight" );
	listItemHeight			= registry.readIndexInt( "gui" , "listItemHeight" );
	counterHeight			= registry.readIndexInt( "gui" , "counterHeight" );
	selectHeight			= registry.readIndexInt( "gui" , "selectHeight" );
	
	startButtonTextColor	= registry.readIndexInt( "gui" , "startButtonTextColor" );
	fileExtensionVisible	= registry.readIndexInt( "gui" , "fileExtensionVisible" );
	
	changeState( _guiState::desktop );
	
	return true;
}


void _guiController::frame()
{
	if( _guiController::keyboard )
		_guiController::keyboard->vbl();
	
	// Change the currently active gui if necessary
	if( !dirty )
		return;
	
	// Prevent the vertical blank from interrupting the following few lines of code
	_interruptController::enterCriticalSection();
	
	// Fade down Screen
	_videoController::fadeScreens( false );
	
	// Set Label to jump to
	start:
	
	if( _guiController::activeGui )
		_guiController::activeGui = nullptr; // Delete old activeGui
	
	_guiController::dirty = false;
	
	// Clean up
	_guiController::deleteHost();
	_guiController::disableKeyboard();
	
	// Create new state
	switch( _guiController::state )
	{
		case _guiState::bootup:
			_guiController::activeGui = new _guiBootup();
			break;
		case _guiState::login:
			_guiController::activeGui = new _guiLogin();
			break;
		case _guiState::setup:
			_guiController::activeGui = new _guiSetup();
			break;
		case _guiState::desktop:
			_guiController::activeGui = new _guiDesktop();
			break;
		default:
			break;
	}
	
	if( _guiController::dirty )
		goto start;
	
	// Fade in the screen
	if( _guiController::state != _guiState::empty )
		_videoController::fadeScreens( true );
	
	_interruptController::leaveCriticalSection(); // Enable the vertical blank interrupt again
}

void _guiController::enableKeyboard( _optValue<_coord> handlePosition ){
	if( !_guiController::keyboard )
		_guiController::keyboard = new _keyboardScreen( _videoController::getBgIdKeyboard() , move(handlePosition) );
}

void _guiController::disableKeyboard(){
	if( _guiController::keyboard ){
		_interruptController::enterCriticalSection();
		_guiController::keyboard = nullptr;
		_interruptController::leaveCriticalSection();
	}
}

void _guiController::end()
{
	// Delete Top Screen
	_guiController::top = nullptr;
	
	// Clean up the current gui state
	_guiController::changeState( _guiState::empty );
	_guiController::frame(); // Makes sure this state-change gets processed
	
	// Write Some Attributes
	_ini& registry = _registryController::getSystemRegistry();
	registry.writeIndex( "gui" , "minDragDistance" , int2string(minDragDistance) );
	registry.writeIndex( "gui" , "maxClickPeriod" , int2string(maxClickPeriod) );
	registry.writeIndex( "gui" , "keyRepetitionDelay" , int2string(keyRepetitionDelay) );
	registry.writeIndex( "gui" , "keyRepetitionPause" , int2string(keyRepetitionPause) );
	registry.writeIndex( "gui" , "maxDoubleClickPeriod" , int2string(maxDoubleClickPeriod) );
	registry.writeIndex( "gui" , "maxDoubleClickArea" , int2string(maxDoubleClickArea) );
	registry.writeIndex( "gui" , "rightClickDelay" , int2string(rightClickDelay) );
	
	registry.writeIndex( "gui" , "fileObjectHeight" , int2string(fileObjectHeight) );
	registry.writeIndex( "gui" , "listItemHeight" , int2string(listItemHeight) );
	registry.writeIndex( "gui" , "counterHeight" , int2string(counterHeight) );
	registry.writeIndex( "gui" , "selectHeight" , int2string(selectHeight) );
	
	registry.writeIndex( "gui" , "startButtonTextColor" , int2string(startButtonTextColor) );
	registry.writeIndex( "gui" , "fileExtensionVisible" , int2string(fileExtensionVisible) );
}

_guiState					_guiController::state = _guiState::empty;
bool						_guiController::dirty = false;
_uniquePtr<_gui>			_guiController::activeGui;
_uniquePtr<_gadgetScreen>	_guiController::host;
_uniquePtr<_keyboardScreen>	_guiController::keyboard;
_uniquePtr<_screen>			_guiController::top;
_gadget*					_guiController::currentFocus;
_gadget*					_guiController::lastClickedGadget;
_bitmap						_guiController::windowBar;
_bitmap						_guiController::windowBarBlurred;
_color						_guiController::itemFg;
_color						_guiController::itemBg;
_color						_guiController::focusFg;
_color						_guiController::focusBg;
_color						_guiController::selectFg;
_color						_guiController::selectBg;
_color						_guiController::controlBg;
_color						_guiController::controlFg;
bool						_guiController::magnifyKeyboardFocus;
_u16 						_guiController::minDragDistance;
_u16						_guiController::maxClickPeriod;
_u16						_guiController::keyRepetitionDelay;
_u16						_guiController::keyRepetitionPause;
_u8							_guiController::maxDoubleClickPeriod;
_u8							_guiController::maxDoubleClickArea;
_u16						_guiController::rightClickDelay;
_u8							_guiController::fileObjectHeight;
_u8							_guiController::listItemHeight;
_length						_guiController::counterHeight;
_length						_guiController::selectHeight;
_color						_guiController::startButtonTextColor;
bool						_guiController::fileExtensionVisible;