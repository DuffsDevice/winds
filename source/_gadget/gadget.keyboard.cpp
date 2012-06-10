#include "_gadget/gadget.keyboard.h"
#include "_gadget/gadget.windows.h"
#include "_type/type.animation.h"
#include "_type/type.system.h"

#include <nds.h>

const int sStart = 0;
const int sSwap = 13;
const int sEnd = 49;

//! Screen_Height etc...
#include "nds/arm9/console.h"

#include "_graphic/BMP_Background.h"

//_bitmap* _keyboard::bg = new BMP_Background();

_rect _keyboard::buttonDimensions[]{
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

void _keyboard::setState( int value )
{
	bgSetScrollf( 3 , 0 , int(value*1.32) << 8 );
	bgUpdate();
	
	// Move it relatively
	this->moveTo( 0 , SCREEN_HEIGHT - 10 - value/1.32 );
}

void _keyboard::setDestination( _gadget* dest )
{
	// Altes Ziel den Fokus wieder nehmen
	if( this->destination != nullptr && this->destination->getParent() != nullptr )
		this->destination->triggerEvent( _gadgetEvent( "blur" ) );
	
	this->destination = dest;
	
	//! If not already visible
	if( !this->mode && dest )
	{
		this->anim.setFromValue( sStart );
		this->anim.setToValue( sEnd );
		this->anim.start();
		this->mode = true;
	}
	else if( this->mode && !dest )
	{
		this->anim.setFromValue( sEnd );
		this->anim.setToValue( sStart );
		this->anim.start();
		this->mode = false;
	}
}

_gadgetEventReturnType _keyboard::refreshHandler( _gadgetEvent event )
{
	// Receive Gadget
	_keyboard* that = (_keyboard*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	bP.copyHorizontalStretch( 33 , 0 , SCREEN_WIDTH - 33 , _system_->_runtimeAttributes_->windowBar );
	//bP.copy( 0 , 10 , that->bg );
	
	return use_default;
}

_gadgetEventReturnType _keyboard::keyHandler( _gadgetEvent event )
{	
	// Receive Gadget
	_keyboard* that = (_keyboard*)event.getGadget();
	
	if( event.getType() == "keyClick" && event.getArgs().getKeyCode() == DSWindows::KEY_SHIFT )
	{
		if( that->shift == 2 )
			return handled;
		
		that->shift = !that->shift;
		
		for( _u8 i = 0 ; i < 46 ; i++ )
		{
			that->buttons[i]->setTitle( _system_->_runtimeAttributes_->keyboardText[ bool( that->shift ) ][i] );
			that->buttons[i]->setKey( _system_->_runtimeAttributes_->keyboardChar[ bool( that->shift ) ][i] );
		}
	}
	else if( that->destination != nullptr )
		that->destination->handleEvent( event );
	
	return handled;
}

_gadgetEventReturnType _keyboard::focusHandler( _gadgetEvent event ){
	return handled;
}

_gadgetEventReturnType _keyboard::dragHandler( _gadgetEvent event )
{
	// Receive Gadget
	_keyboard* that = (_keyboard*)event.getGadget();
	
	static int deltaY = 0;
	
	if( event.getType() == "dragStart" )
	{
		// If y pos is not on the windowbar, let my children gagdet be the object of Dragment :-)
		if( event.getArgs().getPosY() > 11 )
		{
			that->dragMe = false;			
			// Check children
			return use_default;
		}
		
		that->dragMe = true;
		
		deltaY = event.getArgs().getPosY();
		
		// If y is on the windowbar, drag Me!
		return handled;
	}
	else if( event.getType() == "dragging" )
	{
		// Check if there is a gadget who receives drag-events,
		// If not, it has to be me who's dragged
		if( !that->dragMe )
			return use_default;
		
		/**
		 * Handling of my 'dragment' !
		**/
		
		// Has the Gadget to move?
		if( event.getArgs().getDeltaY() == 0 )
			return handled;
		
		that->setState( mid( ( SCREEN_HEIGHT - 10 - event.getArgs().getPosY() + (bgState[3].scrollY>>8) + deltaY )/2 , sEnd , sStart ) );
		
		// Return
		return handled;
	}
	else if( event.getType() == "dragStop" )
	{
		// Check if there is a gadget who receives drag-events,
		// If not, it has to be me who's dragged
		if( !that->dragMe )
			return use_default;
		
		_u8 state = mid( ( SCREEN_HEIGHT - 10 - event.getArgs().getPosY() + (bgState[3].scrollY>>8) + deltaY )/2 , sEnd , sStart );
		
		that->anim.setFromValue( state );
		
		if( ( that->mode == true && state < sEnd - sSwap ) || ( that->mode == false && state < sStart + sSwap ) ){
			that->anim.setToValue( sStart );
			that->mode = false;
			// Altes Ziel den Fokus wieder nehmen
			if( that->destination != nullptr && that->destination->getParent() != nullptr )
				that->destination->triggerEvent( _gadgetEvent( "blur" ) );
			that->destination = nullptr;
		}
		else{
			that->anim.setToValue( sEnd );
			that->mode = true;
		}
		
		that->anim.start();
		
		// Return
		return handled;
	}
	
	// Default return
	return not_handled;
}

_keyboard::_keyboard( _gadgetStyle style ) :
	_gadget( _gadgetType::keyboard , SCREEN_WIDTH , 112 , 0 , SCREEN_HEIGHT - 10 , style )
	, font( new FONT_CourierNew10() )
	, startButton( new _keyboardStartButton( 0 , 0 ) )
	, shift( false )
	, mode( false ) // Means "Hidden"
	, destination( nullptr )
	, anim( 0 , 0 , 500 )
{
	this->bitmap->reset( RGB( 19,19,19) );
	//! Create the buttons
	for( _u8 i = 0 ; i < 46 ; i++ )
	{
		this->buttons[i] = new _keyboardButton( _system_->_runtimeAttributes_->keyboardChar[0][i] , this->buttonDimensions[i].width , this->buttonDimensions[i].height , this->buttonDimensions[i].x , this->buttonDimensions[i].y + 10 , _system_->_runtimeAttributes_->keyboardText[0][i] );
		this->buttons[i]->setFont( this->font );
		this->addChild( this->buttons[i] );
	}
	
	this->buttons[45]->setAutoSelect( true );
	this->buttons[40]->setAutoSelect( true );
	
	//! Animations
	this->anim.setEasing( _animation::_bounce::easeOut );
	this->anim.setter( [&]( int y ){ this->setState( y ); } );
	
	//! Register my handler as the default Refresh-Handler
	this->unregisterEventHandler( "mouseDoubleClick" );
	this->registerEventHandler( "refresh" , &_keyboard::refreshHandler );
	this->registerEventHandler( "keyDown" , &_keyboard::keyHandler );
	this->registerEventHandler( "keyUp" , &_keyboard::keyHandler );
	this->registerEventHandler( "keyClick" , &_keyboard::keyHandler );
	
	this->registerEventHandler( "dragStart" , &_keyboard::dragHandler );
	this->registerEventHandler( "dragStop" , &_keyboard::dragHandler );
	this->registerEventHandler( "dragging" , &_keyboard::dragHandler );
	
	this->registerEventHandler( "focus" , &_keyboard::dragHandler );
	
	this->addChild( this->startButton );
	
	// Refresh Me
	this->refreshBitmap();
}

_keyboard::~_keyboard()
{
	delete this->font;
	
	for( _u8 i = 0 ; i < 46 ; i++ )
		delete this->buttons[i];
}