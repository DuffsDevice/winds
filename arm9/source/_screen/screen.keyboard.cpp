#include <_screen/screen.keyboard.h>
#include <_screen/screen.desktop.h>
#include <_type/type.animation.h>
#include <_controller/controller.font.h>
#include <_controller/controller.gui.h>
#include <_controller/controller.localization.h>
#include <_controller/controller.memory.h>

// NDS Includes
extern "C"{
	#undef unused
	#undef deprecated
	#include <nds/interrupts.h>
	#include <nds/arm9/background.h>
	#include <nds/arm9/video.h>
}

// Resources
#include <_resource/resource.image.keyboard.grip.h>

//! Define if you want the upper screen to show
//! the full lower screen when keyboard is expanded
//#define KEYBOARD_TOP_SCREEN

namespace{
	const int sStart = 0;
	const int sSwap = 34;
	const int sEnd = 108;
}

_rect _keyboardScreen::buttonDimensions[]{
	_rect( 2 , 2 , 22 , 18 ), // 1
	_rect( 26 , 2 , 22 , 18 ), // 2
	_rect( 50 , 2 , 22 , 18 ), // 3
	_rect( 74 , 2 , 22 , 18 ), // 4
	_rect( 98 , 2 , 22 , 18 ), // 5
	_rect( 122 , 2 , 22 , 18 ), // 6
	_rect( 146 , 2 , 22 , 18 ), // 7
	_rect( 170 , 2 , 22 , 18 ), // 8
	_rect( 194 , 2 , 22 , 18 ), // 9
	_rect( 218 , 2 , 22 , 18 ), // 0
	_rect( 9 , 22 , 22 , 18 ), // Q
	_rect( 33 , 22 , 22 , 18 ), // W
	_rect( 57 , 22 , 22 , 18 ), // E
	_rect( 81 , 22 , 22 , 18 ), // R
	_rect( 105 , 22 , 22 , 18 ), // T
	_rect( 129 , 22 , 22 , 18 ), // Y/Z
	_rect( 153 , 22 , 22 , 18 ), // U
	_rect( 177 , 22 , 22 , 18 ), // I
	_rect( 201 , 22 , 22 , 18 ), // O
	_rect( 225 , 22 , 22 , 18 ), // P
	_rect( 2 , 42 , 22 , 18 ), // A
	_rect( 26 , 42 , 22 , 18 ), // S
	_rect( 50 , 42 , 22 , 18 ), // D
	_rect( 74 , 42 , 22 , 18 ), // F
	_rect( 98 , 42 , 22 , 18 ), // G
	_rect( 122 , 42 , 22 , 18 ), // H
	_rect( 146 , 42 , 22 , 18 ), // J
	_rect( 170 , 42 , 22 , 18 ), // K
	_rect( 194 , 42 , 22 , 18 ), // L
	_rect( 218 , 42 , 36 , 18 ), // Backspace
	_rect( 2 , 62 , 28 , 18 ), // Caps-Lock
	_rect( 32 , 62 , 22 , 18 ), // Z/Y
	_rect( 56 , 62 , 22 , 18 ), // X
	_rect( 80 , 62 , 22 , 18 ), // C
	_rect( 104 , 62 , 22 , 18 ), // V
	_rect( 128 , 62 , 22 , 18 ), // B
	_rect( 152 , 62 , 22 , 18 ), // N
	_rect( 176 , 62 , 22 , 18 ), // M
	_rect( 200 , 62 , 22 , 18 ), // .
	_rect( 224 , 62 , 30 , 18 ), // Return
	_rect( 2 , 82 , 33 , 18 ), // Shift
	_rect( 37 , 82 , 22 , 18 ), // Windows
	_rect( 61 , 82 , 110 , 18 ), // Space
	_rect( 173 , 82 , 22 , 18 ), // ,
	_rect( 197 , 82 , 22 , 18 ), // ?
	_rect( 221 , 82 , 33 , 18 ) // Shift
};

void _keyboardScreen::vbl()
{
	_gadget* active = _guiController::getCurrentFocus();
	
	#ifdef KEYBOARD_TOP_SCREEN
		_screen* topScreen = _guiController::getTopScreen();
		_screen* hostScreen = _guiController::getHost();
		if( topScreen && hostScreen ) // Copy Screen to top
		{
			memcpy16( topScreen->getMemoryPtr() , hostScreen->getMemoryPtr() , SCREEN_WIDTH * SCREEN_HEIGHT );
			if( active ){
				_bitmap b = _bitmap( topScreen->getMemoryPtr() , SCREEN_WIDTH , SCREEN_HEIGHT );
				_rect dim = _guiController::getCurrentFocus()->getAbsoluteDimensions();
				b.drawDottedRect( dim.x , dim.y , dim.width , dim.height , _guiController::getItemBg( true ) );
			}
		}
	#endif
	
	if(
		this->lastCurrentFocus != active // If the focused gadgets changed
		|| (
			!this->manuallyOpened
			&& this->mode !=
			(
				active
				&& active->requestsKeyboard() // or if a gadgets settings were changed
			)
		)
	) 
	{
		this->lastCurrentFocus = active;
		
		if( this->ignoreNextVBL )
		{
			this->ignoreNextVBL = false;
			return;
		}
		
		if( active && active->requestsKeyboard() )
		{
			bool wasManuallyOpened = this->manuallyOpened; // if the keyboard was previously opened by hand we only zoom to the currently focused gadget
			this->open();
			this->manuallyOpened = wasManuallyOpened;
		}
		else if( !this->manuallyOpened )
			this->close();
		else
			this->resetZoom();
	}
}

void _keyboardScreen::setState( int value )
{
	if( value != this->curState )
	{
		float percent = float( value ) / sEnd;
		
		_u8 keyboardScroll = this->fromKeyboardExpansion + float( this->toKeyboardExpansion - this->fromKeyboardExpansion ) * percent + 0.5f;
		
		// Scroll Keyboard
		this->scrollY( keyboardScroll - SCREEN_HEIGHT + 9 );
		
		#ifdef KEYBOARD_TOP_SCREEN
			_screen* topScreen = _guiController::getTopScreen();
			if( topScreen ) // Topper Screen
			{
				topScreen->scrollY( keyboardScroll * 1.77f - SCREEN_HEIGHT );
				REG_BLDCNT_SUB = ( 1 << 3 ) // 3rd Screen_Layer
				| ( 3 << 6 ) ; // Det Blend Mode to fade to black ( 2 << 6 ) would be fading to white
				REG_BLDY_SUB = 31 - float( keyboardScroll * 31 ) / sEnd;
			}
		#endif
		
		_screen* hostScreen = _guiController::getHost();
		if( hostScreen )
		{
			//! Scale Main-Screen
			float rat = this->fromFactor + percent * ( this->toFactor - this->fromFactor );
			hostScreen->scale( rat );
			
			hostScreen->scrollX( this->fromX + percent * ( this->toX - this->fromX ) + 0.5f );
			hostScreen->scrollY( this->fromY + percent * ( this->toY - this->fromY ) + 0.5f );
		}
		
		bgUpdate();
		
		// Set Current State
		this->curState = value;
	}
}

void _keyboardScreen::prepareAnimation()
{
	float percent = float( this->curState ) / sEnd;
	this->fromFactor = this->fromFactor + percent * ( this->toFactor - this->fromFactor );
	this->fromX = this->fromX + float( this->toX - this->fromX ) * percent + 0.5f;
	this->fromY = this->fromY + float( this->toY - this->fromY ) * percent + 0.5f;
	this->fromKeyboardExpansion = this->fromKeyboardExpansion + float( this->toKeyboardExpansion - this->fromKeyboardExpansion ) * percent + 0.5f;
	
	//! Prepare 'anim'
	this->anim.setFromValue( sStart );
	this->anim.setToValue( sEnd );
}

void _keyboardScreen::resetZoom( bool useAnim )
{
	// Set s all 'from'-values
	this->prepareAnimation();
	
	// Set Animation parameters
	this->toX = 0;
	this->toY = sEnd;
	this->toFactor = 1;
	this->toKeyboardExpansion = sEnd;
	
	_gadget* active = _guiController::getCurrentFocus();
	
	// Altem Ziel den Fokus wieder nehmen
	if( active != nullptr && active->requestsKeyboard() )
		active->blur();
	
	// Reset Zoom
	if( useAnim )
		this->anim.start();
	else
	{
		anim.terminate();
		this->curState = sStart;
		this->setState( sEnd );
	}
}

void _keyboardScreen::close( bool useAnim )
{
	// Set Animation parameters
	this->prepareAnimation();
	this->toX = 0;
	this->toY = 0;
	this->toFactor = 1;
	this->toKeyboardExpansion = sStart;
	
	_gadget* active = _guiController::getCurrentFocus();
	
	// Altem Ziel den Fokus wieder nehmen
	if( active && active->requestsKeyboard() )
	{
		// Set 'ignoreNextVBL' to true to prevent that the keyboardVBL closes the keyboard a second time
		// since the currentFocus changes probably to some gadget that doesn't require keyboard
		// This would ruin the whole keyboard animation
		this->ignoreNextVBL = true;
		active->deselect();
	}
	
	this->manuallyOpened = false;
	
	this->mode = false; // Indicate we have closen the keyboard
	
	if( this->toX == this->fromX && this->toY == this->fromY && this->toFactor == this->fromFactor && this->toKeyboardExpansion == this->fromKeyboardExpansion )
		return;
	
	// Reset Keyboard
	if( useAnim )
		this->anim.start();
	else
	{
		this->anim.terminate();
		this->curState = sStart;
		this->setState( sEnd );
	}
}

void _keyboardScreen::open( bool useAnim )
{
	// Prepare animation
	this->prepareAnimation();
	
	// Also zoom to a gadget?
	if( this->lastCurrentFocus && this->lastCurrentFocus->requestsKeyboard() )
	{
		_rect dim = this->lastCurrentFocus->getAbsoluteDimensions();
		
		float myRatio = 256./(SCREEN_HEIGHT-108);
		float itsRatio = float(dim.width)/dim.height;
		float ratio = 1;
		
		// If magnification requested
		if( _guiController::isKeyboardMagnifEnabled() )
		{
			// "higher" than me
			if( itsRatio < myRatio )
				ratio = (SCREEN_HEIGHT-108)/float(dim.height+16);
			else
				ratio = (SCREEN_WIDTH)/float(dim.width+16);
			
			if( ratio > 4 )
				ratio = 4;
			else if( ratio > 3 )
				ratio = 3;
			else if( ratio > 2 )
				ratio = 2;
			else
				ratio = 1;
		}
		
		// Compute reciprocal
		this->toFactor = 1./ratio;
		
		// Compute size of borders around the focused API-Element
		_s8 magnBorderX = ( SCREEN_WIDTH * this->toFactor - dim.width ) / 2;
		_s8 magnBorderY = ( ( SCREEN_HEIGHT - 108 /** Height of Visible Area */ ) * this->toFactor - dim.height ) / 2;
		
		// Clip the computed values so that no areas beside the screen are shown
		this->toX = min( dim.x , _coord( SCREEN_WIDTH - magnBorderX - dim.width - 1 ) ) - magnBorderX;
		this->toX = max( 0 , this->toX );
		
		// Same here...
		this->toY = min( dim.y , _coord( SCREEN_HEIGHT - magnBorderY - dim.height - 1 ) ) - magnBorderY;
		this->toY = max( 0 , this->toY );
	}
	else
	{
		this->toY = sEnd;
		this->toX = 0;
		this->toFactor = 1;
	}
	
	// Open the keyboard!
	this->toKeyboardExpansion = sEnd;
	
	this->mode = true;
	
	if( this->toX == this->fromX && this->toY == this->fromY && this->toFactor == this->fromFactor && this->toKeyboardExpansion == this->fromKeyboardExpansion )
		return;
	
	this->manuallyOpened = true;
	
	// Open Keyboard
	if( useAnim )
		this->anim.start();
	else
	{
		anim.terminate();
		this->curState = sStart;
		this->setState( sEnd );
	}
}

_callbackReturn _keyboardScreen::refreshHandler( _event event )
{
	// Receive Gadget
	_keyboardScreen* that = event.getGadget<_keyboardScreen>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	//! Unused
	//bP.copyTransparent( SCREEN_WIDTH - 40 , 0 , BMP_Grip() );
	//bP.drawFilledRect( 0 , 9 , SCREEN_WIDTH , 112 , _color::fromRGB(19,19,19) );
	//bP.drawHorizontalLine( 0 , 9+0 , SCREEN_WIDTH - 38 , _color::fromRGB( 3 , 3 , 3 ) );
	//bP.drawHorizontalLine( SCREEN_WIDTH - 38 , 9+0 , 28 , _color::fromRGB( 12 , 12 , 12 ) );
	//bP.drawHorizontalLine( SCREEN_WIDTH - 10 , 9+0 , 10 , _color::fromRGB( 3 , 3 , 3 ) );
	
	if( that->handlePosition.isValid() )
	{
		bP.copyTransparent( that->handlePosition , 0 , BMP_Grip() );
		bP.drawHorizontalLine( 0 , 9+0 , that->handlePosition + 2 , _color::fromRGB(2,2,2) );
		bP.drawHorizontalLine( that->handlePosition + 2 , 9+0 , 28 , _color::fromRGB( 12 , 12 , 12 ) );
		bP.drawHorizontalLine( that->handlePosition + 30, 9+0 , SCREEN_WIDTH - that->handlePosition - 30 , _color::fromRGB(2,2,2) );
	}
	else
		bP.drawHorizontalLine( 0 , 9+0 , SCREEN_WIDTH , _color::fromRGB( 2 , 2 , 2 ) );
	
	// Background
	bP.drawFilledRect( 0 , 10 , SCREEN_WIDTH , 111 , _color::fromRGB( 19 , 19 , 19 ) );
	bP.drawHorizontalLine( 0 , 9+1 , SCREEN_WIDTH , _color::fromRGB( 12 , 12 , 12 ) );
	bP.drawHorizontalLine( 0 , 9+2 , SCREEN_WIDTH , _color::fromRGB( 14 , 14 , 14 ) );
	bP.drawHorizontalLine( 0 , 9+3 , SCREEN_WIDTH , _color::fromRGB( 16 , 16 , 16 ) );
	bP.drawHorizontalLine( 0 , 9+4 , SCREEN_WIDTH , _color::fromRGB( 18 , 18 , 18 ) );
	
	return use_default;
}

_callbackReturn _keyboardScreen::updateHandler( _event event )
{
	_keyboardScreen* that = event.getGadget<_keyboardScreen>();
	
	bool useShiftMap = that->shift != that->caps;
	
	auto textmap = _localizationController::getKeyboardTexts( useShiftMap );
	auto charmap = _localizationController::getKeyboardKeys( useShiftMap );
	
	int i = 0;
	
	for( _gadget* gadget : that->getChildren(false) )
	{
		_keyboardScreenButton* btn = (_keyboardScreenButton*)gadget;
		
		/* Caps Lock */
		if( i == 30 )
			btn->setStrValue( _localizationController::getKeyboardTexts( that->caps )[ i ] );
		
		/* Shift */
		else if( i == 45 || i == 40 )
			btn->setStrValue( _localizationController::getKeyboardTexts( that->shift )[ i ] );
		
		/* Every Other Key */
		else{
			btn->setStrValue( textmap[i] );
			btn->setKey( (_key) charmap[i] );
		}
		i++;
	}
	
	return handled;
}

_callbackReturn _keyboardScreen::keyHandler( _event event )
{	
	// Receive Gadget
	_keyboardScreen* that = event.getGadget<_keyboardScreen>();
	
	if( event == onKeyClick )
	{
		switch( event.getKeyCode() )
		{
			case _key::shift:
				that->shift = !that->shift;
				that->update();
				return handled;
			case _key::caps:
				that->caps = !that->caps;
				that->update();
				return handled;
			case _key::windows:
				if( _guiController::getHost() && _guiController::getHost()->getScreenType() == _gadgetScreenType::desktop ){
					_desktopScreen* win = (_desktopScreen*)_guiController::getHost();
					
					if( win->isStartMenuOpened() )
						win->closeStartMenu();
					else
						win->openStartMenu();
				}
				return handled;
			case _key::backspace:
			case _key::carriage_return:
				break;
			default:
				if( that->shift ){
					// Remove Shift
					that->shift = false;
					that->update();
				}
		}
	}
	else{
		// Don't forward those key-types
		switch( event.getKeyCode() ){
			case _key::shift:
			case _key::caps:
			case _key::windows:
				return handled;
			default:
				break;
		}
	}
	
	if( that->lastCurrentFocus )
		that->lastCurrentFocus->triggerEvent( event );
	else if( _guiController::getHost() )
		_guiController::getHost()->triggerEvent( event );
	
	return handled;
}

_callbackReturn _keyboardScreen::dragHandler( _event event )
{
	// Receive Gadget
	_keyboardScreen* that = event.getGadget<_keyboardScreen>();
	
	static int deltaY = 0;
	
	if( event == onDragStart )
	{
		// If y pos is not on the windowbar, let my children gagdet be the object of Dragment :-)
		if( event.getPosY() > 11 ){
			that->dragMe = false;
			return use_default;
		}
		
		that->dragMe = true;
		
		// Obtain offset of drag to left-top-corner
		deltaY = event.getPosY();
		
		// Prepare the animation
		that->prepareAnimation();
		
		// Set Destination
		if( that->mode )
		{
			that->toY = sStart;
			that->toX = 0;
			that->toFactor = 1;
			that->toKeyboardExpansion = sStart;
		}
		else
		{
			that->toY = sEnd;
			that->toX = 0;
			that->toFactor = 1;
			that->toKeyboardExpansion = sEnd;
		}
	}
	else if( event == onDragging )
	{		
		// Check if there is a gadget who receives drag-events,
		// If not, it has to be me who's dragged
		if( !that->dragMe )
			return use_default;
		
		// Handling of my 'dragment'!
		_s16 tempTouch = SCREEN_HEIGHT - event.getEffectivePosY() + deltaY - 10;
		
		_s16 val = float( ( tempTouch - that->fromKeyboardExpansion ) * sEnd ) / float( that->toKeyboardExpansion - that->fromKeyboardExpansion );
		val = mid( val , (_s16)sEnd , (_s16)sStart );
		
		that->setState( val );
	}
	else if( event == onDragStop )
	{
		// Check if there is a gadget who receives drag-events,
		// If not, it has to be me who's dragged
		if( !that->dragMe )
			return use_default;
		
		float percent = float( that->curState ) / sEnd;
		
		_u8 keyboardScroll = that->fromKeyboardExpansion + float( that->toKeyboardExpansion - that->fromKeyboardExpansion ) * percent + 0.5f;
		
		if( 
			( that->mode && keyboardScroll < sEnd - sSwap ) // Opened and underneath the trigger to open up the keyboard (~ 2/3 of the over all height)
			|| ( !that->mode && keyboardScroll < sStart + sSwap ) // Closed and above the point where the keyboard opens (~ 1/3 of the over all height)
		)
			that->close();
		else
			that->open();
		
		that->dragMe = false;
	}
	
	// Default return
	return handled;
}

_keyboardScreen::_keyboardScreen( _u8 bgId , _optValue<_coord> position , _style&& style ) :
	_gadgetScreen( bgId , _gadgetScreenType::keyboard , SCREEN_WIDTH , /* Height */ sEnd + 10 , style | _style::canNotTakeFocus | _style::notFocusable )
	, fromX( 0 )
	, fromY( 0 )
	, fromFactor( 1 )
	, fromKeyboardExpansion( 0 )
	, toX( 0 )
	, toY( 0 )
	, toFactor( 1 )
	, toKeyboardExpansion( 0 )
	, curState( -1 )
	, handlePosition( move(position) )
	, manuallyOpened( false )
	, ignoreNextVBL( false )
	, shift( false )
	, caps( false )
	, mode( false ) // Means "Hidden"
	, anim( 0 , 0 , 800 )
{
	_fontHandle fnt			= _fontController::getFont( "CourierNew10" );
	_fontHandle systemFont	= _fontController::getFont( "System7" );
	const auto& textmap		= _localizationController::getKeyboardTexts( false );
	const auto& charmap		= _localizationController::getKeyboardKeys( false );
	
	// Create the buttons
	for( _s8 i = 45 ; i >= 0 ; i-- )
	{
		_keyboardScreenButton* btn = new _keyboardScreenButton( this->buttonDimensions[i].x , this->buttonDimensions[i].y + 14 , this->buttonDimensions[i].width , this->buttonDimensions[i].height , (_key)charmap[i] , textmap[i] );
		
		switch( i )
		{
			case 45: // Right Shift
			case 40: // Left Shift
				btn->setAutoSelect( true );
				btn->applyStyle( _style::noClickRepeat );
			case 41: // Windows-Button
			case 39: // Carriage Return
			case 30: // Caps
			case 29: // Backspace
				btn->setFont( systemFont );
				break;
			default:
				btn->setFont( fnt );
				break;
		}
		this->addChild( btn );
	}
	
	// Animations
	this->anim.setEasing( _animation::_expo::easeOut );
	this->anim.setSetter( _classCallback<void(int)>( this , &_keyboardScreen::setState ) );
	
	// Reset Keyboard
	this->setState( 0 );
	
	// Register my handler as the default Refresh-Handler
	this->setInternalEventHandler( onDraw , make_callback( &_keyboardScreen::refreshHandler ) );
	this->setInternalEventHandler( onUpdate , make_callback( &_keyboardScreen::updateHandler ) );
	this->setInternalEventHandler( onKeyDown , make_callback( &_keyboardScreen::keyHandler ) );
	this->setInternalEventHandler( onKeyUp , make_callback( &_keyboardScreen::keyHandler ) );
	this->setInternalEventHandler( onKeyClick , make_callback( &_keyboardScreen::keyHandler ) );
	this->setInternalEventHandler( onKeyRepeat , make_callback( &_keyboardScreen::keyHandler ) );
	
	this->setInternalEventHandler( onDragStart , make_callback( &_keyboardScreen::dragHandler ) );
	this->setInternalEventHandler( onDragStop , make_callback( &_keyboardScreen::dragHandler ) );
	this->setInternalEventHandler( onDragging , make_callback( &_keyboardScreen::dragHandler ) );
	
	// Refresh Me
	this->redraw();
}

_keyboardScreen::~_keyboardScreen()
{
	// Hide Keyboard
	this->close( false );
	
	// Delete Buttons
	this->removeChildren( true );
}

_gadget*	_keyboardScreen::lastCurrentFocus = nullptr;