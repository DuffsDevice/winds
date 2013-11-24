#include "_gadget/gadget.scrollBar.h"
#include "_resource/BMP_ScrollButtons.h"

#include "nds/arm9/math.h"

_bitmap scroll_bg_vert 			= BMP_ScrollBgSnipVertical();
_bitmap scroll_bg_horiz 		= BMP_ScrollBgSnipHorizontal();

_scrollBar::_scrollBar( _optValue<_coord> x , _optValue<_coord> y , _length gadgetLength , _u32 length , _u32 length2 , _dimension dim , _u32 value , _style&& style ) :
	_gadget( 
		_gadgetType::scrollbar
		, x , y 
		, dim == _dimension::horizontal ? gadgetLength : 8
		,  dim == _dimension::vertical ? gadgetLength : 8 
		, (_style&&)style
	)
	, value( 0 )
	, length( length )
	, length2( length2 )
	, step( 1 )
	, dim( dim )
	, anim( 0 , 1 , 600 )
{
	this->anim.setEasing( _animation::_cubic::easeInOut );
	this->anim.setter( make_callback( this , (void(_scrollBar::*)(int)) &_scrollBar::setValueInternal ) );
	
	if( dim == _dimension::horizontal )
	{
		this->dragHandle	= new _scrollButton( 0 , 0 , 8 , 8 , _scrollButtonType::buttonHandleX );
		this->higherHandle	= new _scrollButton( this->getWidth() - 8 , 0 , 8 , 8 , _scrollButtonType::buttonRight );
		this->lowerHandle	= new _scrollButton( 0 , 0 , 8 , 8 , _scrollButtonType::buttonLeft );
	}
	else
	{
		this->dragHandle	= new _scrollButton( 0 , 8 , 8 , 8 , _scrollButtonType::buttonHandleY  );
		this->higherHandle	= new _scrollButton( 0 , this->getHeight() - 8 , 8 , 8 , _scrollButtonType::buttonBottom );
		this->lowerHandle	= new _scrollButton( 0 , 0 , 8 , 8 , _scrollButtonType::buttonTop );
	}
	
	//! Register Event-Handlers
	this->setInternalEventHandler( onDraw , make_callback( &_scrollBar::refreshHandler ) );
	this->setInternalEventHandler( onResize , make_callback( &_scrollBar::resizeHandler ) );
	
	//! Action Handlers
	this->dragHandle->setInternalEventHandler( onDragStart , make_callback( &_scrollBar::dragHandler ) );
	this->dragHandle->setInternalEventHandler( onDragging , make_callback( &_scrollBar::dragHandler ) );
	
	this->higherHandle->setInternalEventHandler( onMouseClick , make_callback( &_scrollBar::clickHandler ) );
	this->lowerHandle->setInternalEventHandler( onMouseClick , make_callback( &_scrollBar::clickHandler ) );
	
	//! Refresh my cached values
	refreshHandleWidth();
	
	// Add Buttons
	this->addEnhancedChild( this->dragHandle );
	this->addEnhancedChild( this->higherHandle );
	this->addEnhancedChild( this->lowerHandle );
	
	this->redraw();
}


_scrollBar::~_scrollBar()
{
	delete this->dragHandle;
	delete this->higherHandle;
	delete this->lowerHandle;
}


void _scrollBar::setDimension( _dimension dim )
{
	if( this->dim == dim )
		return;
	this->dim = dim;
	
	if( dim == _dimension::horizontal )
	{
		this->dragHandle->setButtonType( _scrollButtonType::buttonHandleX );
		this->higherHandle->moveTo( this->getWidth() - 8 , 0 );
		this->higherHandle->setButtonType( _scrollButtonType::buttonRight );
		this->lowerHandle->moveTo( 0 , 0 );
		this->lowerHandle->setButtonType( _scrollButtonType::buttonLeft );
	}
	else
	{
		this->dragHandle->setButtonType( _scrollButtonType::buttonHandleY );
		this->higherHandle->moveTo( 0 , this->getHeight() - 8 );
		this->higherHandle->setButtonType( _scrollButtonType::buttonBottom );
		this->lowerHandle->moveTo( 0 , 0 );
		this->lowerHandle->setButtonType( _scrollButtonType::buttonTop );
	}
	
	this->setValueInternal( this->value );
}


void _scrollBar::setValueInternal( int val )
{
	val = mid( 0 , val , this->length2 - this->length );
	if( val != (int)this->value )
	{
		_s16 delta = val - this->value;
		this->value = val;
		
		this->refreshPosition();
		
		if( this->dim == _dimension::horizontal )
			this->handleEvent( _event( onScroll ).setDeltaX( delta ).setPosX( val ) );
		else
			this->handleEvent( _event( onScroll ).setDeltaY( delta ).setPosY( val ) );
	}
}


void _scrollBar::setValue( _u32 val , bool ease )
{
	if( ease )
	{
		this->anim.terminate();
		this->anim.setFromValue( this->value );
		this->anim.setToValue( mid( 0 , (int)val , int( this->length2 - this->length ) ) );
		this->anim.start();
	}
	else
		this->setValueInternal( val );
}


_callbackReturn _scrollBar::dragHandler( _event event )
{	
	static int deltaY = 0;
	static int deltaX = 0;
	
	if( event == onDragStart )
	{
		deltaY = event.getPosY();
		deltaX = event.getPosX();
		return handled;
	}
	
	// Receive Gadget
	_button* that = event.getGadget<_button>();
	
	_scrollBar* bar = ((_scrollBar*)that->getParent());
	
	// Has the Gadget to move?
	if( !event.getDeltaX() && !event.getDeltaY() )
		return handled;
	
	// Set The value
	if( bar->dim == _dimension::horizontal )
		bar->setValueInternal( div32( ( event.getPosX() - deltaX - 8 ) * bar->length2 << 8 , bar->getWidth() - 15 + bar->cache ) >> 8 );
	else
		bar->setValueInternal( div32( ( event.getPosY() - deltaY - 8 ) * bar->length2 << 8 , bar->getHeight() - 15 + bar->cache ) >> 8 );
	
	return handled;
}


void _scrollBar::refreshHandleWidth()
{
	if( this->length >= this->length2 )
	{
		this->dragHandle->hide();
		return;
	}
	
	this->dragHandle->show();
	
	int length = div32( this->length << 8 , this->length2 ); // Divide, but bit-shift before to get accuracy
	
	int myLength = this->dim == _dimension::horizontal ? this->getWidth() : this->getHeight(); // Dependent on dimensions fetch the length of the scrollbar
	
	// Check if it makes sence to show a dragHandle
	if( myLength < 21 )
		this->dragHandle->hide();
	else
	{
		this->dragHandle->show();
		
		length *= int( myLength - 15 ); // Scale the height to the height of the available scroll area
		
		length = ( length + 16 ) >> 8; // Bit shift right-> we get back our raw value
		
		int i = length;
		length = max( 5 , length ); // Limit the length of the bar to be at least 5 pixels
		
		// Save the difference, we need it later!
		this->cache = i - length;
		
		if( this->dim == _dimension::horizontal )
			this->dragHandle->setWidth( length );
		else
			this->dragHandle->setHeight( length );
	}
}


_callbackReturn _scrollBar::clickHandler( _event event ) {
	
	// Receive Gadget
	_scrollButton* that = event.getGadget<_scrollButton>();
	
	_scrollBar* bar = ((_scrollBar*)that->getParent());
	
	switch( that->getButtonType() )
	{
		case _scrollButtonType::buttonRight :
		case _scrollButtonType::buttonBottom :
			if( bar->value + bar->length >= bar->length2 )
				break;
			if( bar->value + bar->length + bar->step >= bar->length2 )
				bar->setValueInternal( bar->length2 - bar->length );
			else
				bar->setValueInternal( bar->value + bar->step );
			break;
		case _scrollButtonType::buttonTop :
		case _scrollButtonType::buttonLeft :
			if( bar->value <= 0 )
				break;
			if( (int)bar->value - (int)bar->step < 0 )
				bar->setValueInternal( 0 );
			else
				bar->setValueInternal( bar->value - bar->step );
			break;
		default:
			break;
	}
	
	return handled;
}


_callbackReturn _scrollBar::refreshHandler( _event event )
{
	// Receive Gadget
	_scrollBar* that = event.getGadget<_scrollBar>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( COLOR_WHITE );
	
	// Show Scrollbar-Backgrounds
	if( that->dim == _dimension::horizontal )
		bP.copyHorizontalStretch( 8 , 0 , that->getWidth() - 16 , scroll_bg_vert );
	else
		bP.copyVerticalStretch( 0 , 8 , that->getHeight() - 16 , scroll_bg_horiz );
	
	return use_default;
}


_callbackReturn _scrollBar::resizeHandler( _event event )
{
	_scrollBar* that = event.getGadget<_scrollBar>();
	
	that->refreshHandleWidth();
	that->refreshPosition();
	
	if( that->dim == _dimension::horizontal )
		that->higherHandle->setX( that->getWidth() - 8 );
	else
		that->higherHandle->setY( that->getHeight() - 8 );
	
	return handled;
}


void _scrollBar::refreshPosition()
{
	_u32 perc = div32( this->value << 8 , this->length2 );
	
	if( this->dim == _dimension::horizontal )
		this->dragHandle->setX( 8 + ( ( perc * ( this->getWidth() - 15 + this->cache ) + 16 ) >> 8 ) );
	else
		this->dragHandle->setY( 8 + ( ( perc * ( this->getHeight() - 15 + this->cache ) + 16 ) >> 8 ) );
}


void _scrollBar::setLength( _u32 value )
{
	if( this->length == value )
		return;
	
	this->length = value;
	refreshHandleWidth();
	this->setValueInternal( max( _u32(0) , min( this->value , this->length - this->length2 ) ) );
}


void _scrollBar::setLength2( _u32 value )
{
	if( this->length2 == value )
		return;
	
	this->length2 = value;
	refreshHandleWidth();
	this->setValueInternal( max( _u32(0) , min( this->value , this->length2 - this->length ) ) );
}