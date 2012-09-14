#include "_gadget/gadget.keyboard.h"
#include "_gadget/gadget.windows.h"
#include "_type/type.animation.h"
#include "_type/type.system.h"

#include "_resource/BMP_Grip.h"

static _bitmap* Grip = new BMP_Grip();

const int sStart = 0;
const int sSwap = 34;
const int sEnd = 108;

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
	dmaCopyHalfWords( 0 , this->gHScreen->getMemoryPtr() ,  this->topScreen->getMemoryPtr() , 256*192*2 );
	if( this->destination ){
		_bitmap b = _bitmap( this->topScreen->getMemoryPtr() , SCREEN_WIDTH , SCREEN_HEIGHT );
		_rect dim = this->destination->getAbsoluteDimensions();
		b.drawRect( dim.x-4 , dim.y-4 , dim.width+8 , dim.height+8 , COLOR_RED );
		b.drawRect( dim.x-5 , dim.y-5 , dim.width+10 , dim.height+10 , COLOR_RED );
	}
}

int _x = 0 , _y = 0;
float _factor = 1;

void _keyboard::setState( int value )
{
	if( value != this->curState )
	{
		// Scroll Keyboard
		this->scrollY( ( value - SCREEN_HEIGHT + 9 ) );
		
		// Scroll Windows
		if( _factor == 1 && _x == 0 && _y == 0 )
			this->gHScreen->scrollY( value );
		
		// Topper Screen
		this->topScreen->scaleY( float(value*value)/(sEnd*sEnd) );
		this->topScreen->scrollY( value * 1.88f - SCREEN_HEIGHT );
		
		bgUpdate();
		
		this->setY( SCREEN_HEIGHT - value );
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
	this->gHScreen->scale( rat );
	
	this->gHScreen->scrollX( -percent * ( 0 - _x ) );
	this->gHScreen->scrollY( -percent * ( 0 - _y ) );
	
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
		float itsRatio = float(dim.width)/dim.height;
		float ratio = 1;
		
		// "higher" than me
		if( itsRatio < myRatio )
			ratio = float(dim.height+16)/(SCREEN_HEIGHT-112);
		else
			ratio = float(dim.width+16)/(SCREEN_WIDTH);
		
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
			magnBorder = ( (SCREEN_HEIGHT-112) * ratio - dim.height ) / 2;
		else
			magnBorder = ( SCREEN_WIDTH * ratio - dim.width ) / 2;
			
		_x = dim.x - magnBorder;
		_y = dim.y - magnBorder;
		//! ----------- End -----------
		
		if( !this->mode )
		{
			this->animKeyb.setFromValue( this->curState );
			this->animKeyb.setToValue( sEnd );
			this->animKeyb.start();
			this->mode = true;
		}
		
		this->animMagnif.setFromValue( sStart );
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
	_keyboard* that = event.getGadget<_keyboard>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.resetClippingRects();
	
	bP.copyTransparent( SCREEN_WIDTH - 40 , 0 , Grip );
	bP.drawFilledRect( 0 , 9 , SCREEN_WIDTH , 112 , RGB(19,19,19) );
	bP.drawHorizontalLine( 0 , 9+0 , SCREEN_WIDTH - 38 , RGB( 3 , 3 , 3 ) );
	bP.drawHorizontalLine( SCREEN_WIDTH - 38 , 9+0 , 28 , RGB( 12 , 12 , 12 ) );
	bP.drawHorizontalLine( SCREEN_WIDTH - 10 , 9+0 , 10 , RGB( 3 , 3 , 3 ) );
	bP.drawHorizontalLine( 0 , 9+1 , SCREEN_WIDTH , RGB( 12 , 12 , 12 ) );
	bP.drawHorizontalLine( 0 , 9+2 , SCREEN_WIDTH , RGB( 14 , 14 , 14 ) );
	bP.drawHorizontalLine( 0 , 9+3 , SCREEN_WIDTH , RGB( 16 , 16 , 16 ) );
	bP.drawHorizontalLine( 0 , 9+4 , SCREEN_WIDTH , RGB( 18 , 18 , 18 ) );
	
	return use_default;
}

_gadgetEventReturnType _keyboard::keyHandler( _gadgetEvent event )
{	
	// Receive Gadget
	_keyboard* that = event.getGadget<_keyboard>();
	
	if( event.getArgs().getKeyCode() == DSWindows::KEY_SHIFT )
	{
		if( event.getType() != "keyClick" || that->shift == 2 )
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
	_keyboard* that = event.getGadget<_keyboard>();
	
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
		
		that->animKeyb.terminate();
		that->animMagnif.terminate();
		that->setState( mid( ( SCREEN_HEIGHT - 10 - event.getArgs().getPosY( true ) + deltaY ) , sEnd , sStart ) );
		that->setMagnification( mid( ( SCREEN_HEIGHT - 10 - event.getArgs().getPosY( true ) + deltaY ) , sEnd , sStart ) );
		
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

_keyboard::_keyboard( _u8 bgId , _gadgetScreen* gadgetHost , _screen* topScreen , _gadgetStyle style ) :
	_gadgetScreen( bgId , style )
	, topScreen( topScreen )
	, gHScreen( gadgetHost )
	, font( new FONT_CourierNew10() )
	, shift( false )
	, mode( false ) // Means "Hidden"
	, curState( 1 )
	, destination( nullptr )
	, animKeyb( 0 , 0 , 800 )
	, animMagnif( sStart , sEnd , 800 )
{
	//! Set my bitmap
	this->bitmap->reset( NO_COLOR );
	
	//! Reset Keyboard Position
	this->setState( sStart );
	
	//! Create the buttons
	for( _u8 i = 0 ; i < 46 ; i++ )
	{
		this->buttons[i] = new _keyboardButton( _system_->_runtimeAttributes_->keyboardChar[0][i] , this->buttonDimensions[i].width , this->buttonDimensions[i].height , this->buttonDimensions[i].x , this->buttonDimensions[i].y + 14 , _system_->_runtimeAttributes_->keyboardText[0][i] );
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
	this->registerEventHandler( "refresh" , &_keyboard::refreshHandler );
	this->registerEventHandler( "keyDown" , &_keyboard::keyHandler );
	this->registerEventHandler( "keyUp" , &_keyboard::keyHandler );
	this->registerEventHandler( "keyClick" , &_keyboard::keyHandler );
	
	this->registerEventHandler( "dragStart" , &_keyboard::dragHandler );
	this->registerEventHandler( "dragStop" , &_keyboard::dragHandler );
	this->registerEventHandler( "dragging" , &_keyboard::dragHandler );
	this->registerEventHandler( "focus" , &_keyboard::focusHandler );
	
	// Refresh Me
	this->refreshBitmap();
}

_keyboard::~_keyboard()
{
	delete this->font;
	
	for( _u8 i = 0 ; i < 46 ; i++ )
		delete this->buttons[i];
}