#include "_gadget/gadget.scrollBar.h"
#include "_resource/BMP_ScrollButtons.h"

#include "nds/arm9/math.h"

_bitmap scroll_bg_vert 			= BMP_ScrollBgSnipVertical();
_bitmap scroll_bg_horiz 		= BMP_ScrollBgSnipHorizontal();

_scrollBar::_scrollBar( _coord x , _coord y , _u32 gadgetLength , _u32 length , _u32 length2 , _dimension dim , _u32 value , _style style ) :
	_gadget( _gadgetType::scrollbar , dim == _dimension::horizontal ? gadgetLength : 8 ,  dim == _dimension::vertical ? gadgetLength : 8  , x , y , style ) ,
	value( 0 ) , length( length ) , length2( length2 ) , step( 1 ) , dim( dim ) , anim( 0 , 1 , 600 )
{
	anim.setEasing( _animation::_cubic::easeInOut );
	if( dim == _dimension::horizontal )
	{
		this->dragHandle = new _scrollButton( 8 , 8 , 8 , 0 , _scrollButtonType::buttonHandleX );
		this->higherHandle = new _scrollButton( 8 , 8 , this->dimensions.width - 8 , 0 , _scrollButtonType::buttonRight );
		this->lowerHandle = new _scrollButton( 8 , 8 , 0 , 0 , _scrollButtonType::buttonLeft );
	}
	else
	{
		this->dragHandle = new _scrollButton( 8 , 8 , 0 , 8 , _scrollButtonType::buttonHandleY );
		this->higherHandle = new _scrollButton( 8 , 8 , 0 , this->dimensions.height - 8 , _scrollButtonType::buttonBottom );
		this->lowerHandle = new _scrollButton( 8 , 8 , 0 , 0 , _scrollButtonType::buttonTop );
	}
	
	//! Register Event-Handlers
	this->registerEventHandler( refresh , refreshHandler );
	this->registerEventHandler( onResize , resizeHandler );
	
	this->dragHandle->style.smallDragTrig = true;
	
	//! Action Handlers
	this->dragHandle->registerEventHandler( dragStart , dragHandler );
	this->dragHandle->registerEventHandler( dragging , dragHandler );
	
	this->higherHandle->registerEventHandler( onAction , clickHandler );
	this->lowerHandle->registerEventHandler( onAction , clickHandler );
	
	//! Re-refresh the Buttons with the new event-handlers
	this->dragHandle->refreshBitmap();
	this->higherHandle->refreshBitmap();
	this->lowerHandle->refreshBitmap();
	
	//! Refresh my cached values
	refreshHandleWidth();
	
	// Add Buttons
	this->addChild( this->dragHandle );
	this->addChild( this->higherHandle );
	this->addChild( this->lowerHandle );
	
	this->refreshBitmap();
}

void _scrollBar::setValue( _u32 val )
{
	val = mid( 0 , val , this->length2 - this->length );
	if( val != this->value )
	{
		_s16 delta = val - this->value;
		this->value = val;
		
		this->refreshPosition();
		
		if( this->dim == _dimension::horizontal )
			this->triggerEvent( _event( onChange ).setDeltaX( delta ).setPosX( val ) );
		else
			this->triggerEvent( _event( onChange ).setDeltaY( delta ).setPosY( val ) );
	}
}

void _scrollBar::setValue( _u32 val , bool ease )
{
	if( ease )
	{
		this->anim.terminate();
		this->anim.setFromValue( this->value );
		this->anim.setToValue( mid( 0 , (int)val , int( this->length2 - this->length ) ) );
		this->anim.setter( [&]( _int val ){ this->setValue( val ); } );
		this->anim.start();
	}
	else
		this->setValue( val );
}

_callbackReturn _scrollBar::dragHandler( _event event )
{	
	static int deltaY = 0;
	static int deltaX = 0;
	
	if( event.getType() == dragStart )
	{
		deltaY = event.getPosY();
		deltaX = event.getPosX();
		return handled;
	}
	
	// Receive Gadget
	_button* that = event.getGadget<_button>();
	
	_scrollBar* bar = ((_scrollBar*)that->parent);
	
	// Has the Gadget to move?
	if( !event.getDeltaX() && !event.getDeltaY() )
		return handled;
	
	// Set The value
	if( bar->dim == _dimension::horizontal )
		bar->setValue( div32( ( event.getPosX() - deltaX - 8 ) * bar->length2 << 8 , bar->dimensions.width - 15 + bar->cache ) >> 8 );
	else
		bar->setValue( div32( ( event.getPosY() - deltaY - 8 ) * bar->length2 << 8 , bar->dimensions.height - 15 + bar->cache ) >> 8 );
	
	return handled;
}

void _scrollBar::refreshHandleWidth()
{
	if( this->length >= this->length2 )
	{
		if( this->dim == _dimension::horizontal )
			this->dragHandle->setWidth( int( this->dimensions.width - 15 ) );
		else
			this->dragHandle->setHeight( int( this->dimensions.height - 15 ) );
		return;
	}
		
	int length = div32( this->length << 8 , this->length2 );
	
	if( this->dim == _dimension::horizontal )
		length *= int( this->dimensions.width - 15 );
	else
		length *= int( this->dimensions.height - 15 );
	
	length = ( length + 16 ) >> 8;
	int i = length;
	length = max( 5 , length );
	
	// Save the difference, we need it later!
	this->cache = i - length;
	
	if( this->dim == _dimension::horizontal )
		this->dragHandle->setWidth( length );
	else
		this->dragHandle->setHeight( length );
}

_callbackReturn _scrollBar::clickHandler( _event event ) {
	
	// Receive Gadget
	_button* that = event.getGadget<_button>();
	
	_scrollBar* bar = ((_scrollBar*)that->parent);
	
	switch( that->style.data ){
		
		case 4:
		case 2:
			if( bar->value + bar->length >= bar->length2 )
				break;
			if( bar->value + bar->length + bar->step >= bar->length2 )
				bar->setValue( bar->length2 - bar->length );
			else
				bar->setValue( bar->value + bar->step );
			break;
		case 5:
		case 1:
			if( bar->value <= 0 )
				break;
			if( (int)bar->value - (int)bar->step < 0 )
				bar->setValue( 0 );
			else
				bar->setValue( bar->value - bar->step );
			break;
	}
	
	return handled;
}

_callbackReturn _scrollBar::refreshHandler( _event event )
{
	// Receive Gadget
	_scrollBar* that = event.getGadget<_scrollBar>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	bP.fill( COLOR_WHITE );
	
	// Show Scrollbar-Backgrounds
	if( that->dim == _dimension::horizontal )
		bP.copyHorizontalStretch( 8 , 0 , that->dimensions.width - 16 , scroll_bg_vert );
	else
		bP.copyVerticalStretch( 0 , 8 , that->dimensions.height - 16 , scroll_bg_horiz );
	
	return use_default;
}

_callbackReturn _scrollBar::resizeHandler( _event event ){
	
	_scrollBar* that = event.getGadget<_scrollBar>();
	
	that->refreshHandleWidth();
	that->refreshPosition();
	
	if( that->dim == _dimension::horizontal )
		that->higherHandle->setX( that->dimensions.width - 8 );
	else
		that->higherHandle->setY( that->dimensions.height - 8 );
	
	return handled;
}

void _scrollBar::refreshPosition()
{
	_u32 perc = div32( this->value << 8 , this->length2 );
	
	if( this->dim == _dimension::horizontal )
		this->dragHandle->setX( 8 + ( perc * ( this->dimensions.width - 14 + this->cache ) >> 8 ) );
	else
		this->dragHandle->setY( 8 + ( perc * ( this->dimensions.height - 14 + this->cache ) >> 8 ) );
}

void _scrollBar::setDimension( _dimension dim )
{
	if( dim == this->dim )
		return;
	
	this->dim = dim;
	
	this->unregisterEventHandler( onResize );
	
	if( dim == _dimension::horizontal )
	{
		this->setWidth( this->dimensions.height );
		this->setHeight( 8 );
		this->dragHandle->setDimensions( _rect( 8 , 0 , 8 , 8 ) );
		this->dragHandle->setStyle( _style::storeData( 0 ) );
		this->higherHandle->moveTo( this->dimensions.width - 8 , 0 );
		this->higherHandle->setStyle( _style::storeData( 2 ) );
		this->lowerHandle->setStyle( _style::storeData( 1 ) );
	}
	else
	{
		this->setHeight( this->dimensions.width );
		this->setWidth( 8 );
		this->dragHandle->setDimensions( _rect( 0 , 8 , 8 , 8 ) );
		this->dragHandle->setStyle( _style::storeData( 3 ) );
		this->higherHandle->moveTo( 0 , this->dimensions.height - 8 );
		this->higherHandle->setStyle( _style::storeData( 4 ) );
		this->lowerHandle->setStyle( _style::storeData( 5 ) );
	}
	
	this->registerEventHandler( onResize , resizeHandler );
	
	refreshHandleWidth();
	refreshPosition();
	
	this->bubbleRefresh( true );
}

void _scrollBar::setLength( _u32 value )
{
	if( this->length == value )
		return;
	
	this->length = value;
	refreshHandleWidth();
	this->bubbleRefresh( true );
}

void _scrollBar::setLength2( _u32 value )
{
	if( this->length2 == value )
		return;
	
	this->length2 = value;
	refreshHandleWidth();
}