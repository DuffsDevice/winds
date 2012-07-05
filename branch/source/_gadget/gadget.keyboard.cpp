#include "_gadget/gadget.keyboard.h"
#include "_gadget/gadget.windows.h"
#include "_type/type.animation.h"
#include "_type/type.system.h"

#include <nds.h>

const int sStart = 0;
const int sSwap = 34;
const int sEnd = 102;

//! Screen_Height etc...
#include "nds/arm9/console.h"

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

void _keyboard::screenVBL()
{
	dmaCopyHalfWords( 0 , bgGetGfxPtr(this->winBgId) ,  bgGetGfxPtr(this->bgIdSub) , 256*192*2 );
	if( this->destination ){
		_bitmap b = _bitmap( bgGetGfxPtr(this->bgIdSub) , 256 , 256 );
		_rect dim = this->destination->getAbsoluteDimensions();
		b.drawRect( dim.getX()-4 , dim.getY()-4 , dim.getWidth()+8 , dim.getHeight()+8 , COLOR_RED );
		b.drawRect( dim.getX()-5 , dim.getY()-5 , dim.getWidth()+10 , dim.getHeight()+10 , COLOR_RED );
	}
}

int _x,_y;
float _factor;

void _keyboard::setState( int value )
{
	if( value != this->curState )
	{
		// Scroll Keyboard
		bgSetScrollf( this->bgId , 0 , (value-182) << 8 );
		
		// Scroll Windows
		if( _factor == 1 && _x == 0 && _y == 0 )
			bgSetScrollf( this->winBgId , 0 , value << 8 );
		
		// Topper Screen
		bgSetScale( this->bgIdSub , floatToFixed( 1 , 8 ) , floatToFixed( float(value*value)/(sEnd*sEnd) , 8 ) );
		bgSetScrollf( this->bgIdSub , 0 , int(value*1.88-192) << 8 );
		bgUpdate();
		
		this->setY( SCREEN_HEIGHT - value - 9 );
	}
	this->curState = value;
}

void _keyboard::setMagnification( int value )
{
	
	if( _factor == 1 && _x == 0 && _y == 0 )
		return;
	
	// Scale Lower Screen
	float percent = float(value) / sEnd;
	
	float rat = 1 - percent * ( 1 - _factor );
	bgSetScale( this->winBgId , floatToFixed( rat , 8 ) , floatToFixed( rat , 8 ) );
	
	bgSetScrollf( this->winBgId , int( 0 - percent * ( 0 - _x ) ) << 8 , int( 0 - percent * ( 0 - _y ) ) << 8 );
	
	bgUpdate();
}

void _keyboard::setDestination( _gadget* dest )
{
	// Altes Ziel den Fokus wieder nehmen
	if( this->destination != nullptr && this->destination->getParent() != nullptr )
		this->destination->triggerEvent( _gadgetEvent( "blur" ) );
	
	this->destination = dest;
	
	
	//! If not already visible
	if( dest )
	{
		//! Compute
		_rect dim = this->destination->getAbsoluteDimensions();
		float myRatio = 256./(SCREEN_HEIGHT-112);
		float itsRatio = float(dim.getWidth())/dim.getHeight();
		float ratio = 1;
		
		// "higher" than me
		if( itsRatio < myRatio )
			ratio = float(dim.getHeight()+16)/(SCREEN_HEIGHT-112);
		else
			ratio = float(dim.getWidth()+16)/(SCREEN_WIDTH);
		
		if( ratio < 0.25 )
			ratio = 0.25;
		else if( ratio < 0.3333333 )
			ratio = 0.33333333;
		else if( ratio < 0.5 )
			ratio = 0.5;
		else
			ratio = 1;
		
		// Set 
		_factor = ratio;
		
		_s8 magnBorder = 0;
		
		// "higher" than me
		if( itsRatio < myRatio )
			magnBorder = ( (SCREEN_HEIGHT-112) * ratio - dim.getHeight() ) / 2;
		else
			magnBorder = ( SCREEN_WIDTH * ratio - dim.getWidth() ) / 2;
			
		_x = dim.getX() - magnBorder;
		_y = dim.getY() - magnBorder;
		//! ----------- End -----------
		
		if( !this->mode )
		{
			this->animKeyb.setFromValue( this->curState );
			this->animKeyb.setToValue( sEnd );
			this->animKeyb.start();
			this->mode = true;
		}
		
		this->animMagnif.setFromValue( this->curState );
		this->animMagnif.setToValue( sEnd );
		this->animMagnif.finish();
		this->animMagnif.start();
	}
	else if( this->mode && !dest )
	{
		this->animKeyb.setFromValue( this->curState );
		this->animKeyb.setToValue( sStart );
		this->animKeyb.start();
		this->animMagnif.setFromValue( this->curState );
		this->animMagnif.setToValue( sStart );
		this->animMagnif.finish( [&]( int v ){ _factor = 1; _x = 0; _y = 0; } );
		this->animMagnif.start();
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
		
		that->setState( mid( ( SCREEN_HEIGHT - 10 - event.getArgs().getPosY() + deltaY ) , sEnd , sStart ) );
		that->setMagnification( mid( ( SCREEN_HEIGHT - 10 - event.getArgs().getPosY() + deltaY ) , sEnd , sStart ) );
		
		// Return
		return handled;
	}
	else if( event.getType() == "dragStop" )
	{
		// Check if there is a gadget who receives drag-events,
		// If not, it has to be me who's dragged
		if( !that->dragMe )
			return use_default;
		
		that->animKeyb.setFromValue( that->curState );
		that->animMagnif.setFromValue( that->curState );
		
		if( ( that->mode == true && that->curState < sEnd - sSwap ) || ( that->mode == false && that->curState < sStart + sSwap ) )
		{			
			// Altes Ziel den Fokus wieder nehmen
			if( that->destination != nullptr )
				that->destination->triggerEvent( _gadgetEvent( "blur" ) );
			that->destination = nullptr;
			
			that->animKeyb.setToValue( sStart );
			that->animMagnif.setToValue( sStart );
			that->animMagnif.finish( [&]( int v ){ _factor = 1; _x = 0; _y = 0; } );
			that->mode = false;
		}
		else
		{
			that->animKeyb.setToValue( sEnd );
			that->animMagnif.setToValue( sEnd );
			that->animMagnif.finish();
			that->mode = true;
		}
		
		that->animMagnif.start();
		that->animKeyb.start();
		
		// Return
		return handled;
	}
	
	// Default return
	return not_handled;
}

_keyboard::_keyboard( _u8 bgId , _u8 winBgId , _u8 bgIdSub , _gadgetStyle style ) :
	_gadget( _gadgetType::keyboard , SCREEN_WIDTH , 112 , 0 , SCREEN_HEIGHT - 9 , style , true )
	, bgId( bgId )
	, winBgId( winBgId )
	, bgIdSub( bgIdSub )
	, font( new FONT_CourierNew10() )
	, startButton( new _keyboardStartButton( 0 , 0 ) )
	, shift( false )
	, mode( false ) // Means "Hidden"
	, curState( 1 )
	, destination( nullptr )
	, animKeyb( 0 , 0 , 800 )
	, animMagnif( sStart , sEnd , 800 )
{
	//! Set my bitmap
	this->bitmap = new _bitmap( bgGetGfxPtr(bgId) , SCREEN_WIDTH, SCREEN_HEIGHT );
	this->bitmap->reset( RGB( 19,19,19) );
	this->setState( sStart );
	
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
	this->animKeyb.setEasing( _animation::_expo::easeOut );
	this->animKeyb.setter( [&]( int n ){ this->setState( n ); } );
	this->animMagnif.setEasing( _animation::_expo::easeOut );
	this->animMagnif.setter( [&]( int n ){ this->setMagnification( n ); } );
	
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