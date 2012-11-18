#include "_gadget/gadget.scrollBar.h"
#include "_resource/BMP_ScrollButtons.h"
#include "nds/arm9/math.h"

_bitmap btn_background_released = BMP_ScrollButton();
_bitmap btn_background_pressed 	= BMP_ScrollButtonPressed();
_bitmap scroll_bg_vert 			= BMP_ScrollBgSnipVertical();
_bitmap scroll_bg_horiz 		= BMP_ScrollBgSnipHorizontal();
_pixel 	snip_released_bmp[8] 	= { 64311, 65369, 65368, 65368, 65367, 65367, 64311, 60015 };
_pixel 	snip_pressed_bmp[8]		= { 61008, 63153, 63121, 63122, 63154, 63155, 63187, 64278 };

void drawArrow( _bitmapPort& bP , _u8 dir )
{
	#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
	static _u8 x[10];
	static _u8 y[10];
	switch( dir )
	{
		case 5:
		case 4:
			x[0] = 3;x[1] = 4;x[2] = 2;x[3] = 3;x[4] = 4;x[5] = 5;x[6] = 1;x[7] = 2;x[8] = 5;x[9] = 6;
			break;
		case 2:
			x[0] = 5;x[1] = 5;x[2] = 4;x[3] = 4;x[4] = 4;x[5] = 4;x[6] = 3;x[7] = 3;x[8] = 3;x[9] = 3;
			break;
		case 1:
			x[0] = 2;x[1] = 2;x[2] = 3;x[3] = 3;x[4] = 3;x[5] = 3;x[6] = 4;x[7] = 4;x[8] = 4;x[9] = 4;
			break;
	}
	switch( dir )
	{
		case 5:
			y[0] = 2;y[1] = 2;y[2] = 3;y[3] = 3;y[4] = 3;y[5] = 3;y[6] = 4;y[7] = 4;y[8] = 4;y[9] = 4;
			break;
		case 4:
			y[0] = 5;y[1] = 5;y[2] = 4;y[3] = 4;y[4] = 4;y[5] = 4;y[6] = 3;y[7] = 3;y[8] = 3;y[9] = 3;
			break;
		case 2:
		case 1:
			y[0] = 3;y[1] = 4;y[2] = 2;y[3] = 3;y[4] = 4;y[5] = 5;y[6] = 1;y[7] = 2;y[8] = 5;y[9] = 6;
			break;
	}
	bP.drawPixel( x[0] , y[0] , RGB255( 77 , 97 , 133 ) );
	bP.drawPixel( x[1] , y[1] , RGB255( 77 , 97 , 133 ) );
	bP.drawPixel( x[2] , y[2] , RGB255( 77 , 97 , 133 ) );
	bP.drawPixel( x[3] , y[3] , RGB255( 107 , 129 , 169 ) );
	bP.drawPixel( x[4] , y[4] , RGB255( 107 , 129 , 169 ) );
	bP.drawPixel( x[5] , y[5] , RGB255( 77 , 97 , 133 ) );
	bP.drawPixel( x[6] , y[6] , RGB255( 148 , 164 , 192 ) );
	bP.drawPixel( x[7] , y[7] , RGB255( 107 , 129 , 169 ) );
	bP.drawPixel( x[8] , y[8] , RGB255( 107 , 129 , 169 ) );
	bP.drawPixel( x[9] , y[9] , RGB255( 148 , 164 , 192 ) );
	#pragma GCC diagnostic warning "-Wmaybe-uninitialized"
}
	

_scrollBar::_scrollBar( _coord x , _coord y , _u32 gadgetLength , _u32 length , _u32 length2 , _dimension dim , _u32 value , _style style ) :
	_gadget( _gadgetType::scrollbar , dim == _dimension::horizontal ? gadgetLength : 8 ,  dim == _dimension::vertical ? gadgetLength : 8  , x , y , style ) ,
	value( 0 ) , length( length ) , length2( length2 ) , step( 1 ) , dim( dim )
{
	if( dim == _dimension::horizontal )
	{
		this->dragHandle = new _button( 8 , 8 , 8 , 0 , "" , _style::storeData( _u8(dim) + 0 ) );
		this->higherHandle = new _button( 8 , 8 , this->dimensions.width - 8 , 0 , "" , _style::storeData( _u8(dim) + 2 ) );
		this->lowerHandle = new _button( 8 , 8 , 0 , 0 , "" , _style::storeData( _u8(dim) + 1 ) );
	}
	else
	{
		this->dragHandle = new _button( 8 , 8 , 0 , 8 , "" , _style::storeData( _u8(dim) + 0 ) );
		this->higherHandle = new _button( 8 , 8 , 0 , this->dimensions.height - 8 , "" , _style::storeData( _u8(dim) + 1 ) );
		this->lowerHandle = new _button( 8 , 8 , 0 , 0 , "" , _style::storeData( _u8(dim) + 2 ) );
	}
	
	//! Register Event-Handlers
	this->registerEventHandler( refresh , refreshHandler );
	this->registerEventHandler( onResize , resizeHandler );
	
	this->dragHandle->registerEventHandler( refresh , refreshHandler );
	this->dragHandle->registerEventHandler( dragStart , dragHandler );
	this->dragHandle->registerEventHandler( dragging , dragHandler );
	this->dragHandle->style.smallDragTrig = true;
	
	this->higherHandle->registerEventHandler( refresh , refreshHandler );
	this->higherHandle->registerEventHandler( onAction , clickHandler );
	
	this->lowerHandle->registerEventHandler( refresh , refreshHandler );
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

void _scrollBar::internalSetValue( _u32 val )
{
	val = mid( 0 , (int)val , int( this->length2 - this->length ) );
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
		bar->internalSetValue( div32( ( event.getPosX() - deltaX - 8 ) * bar->length2 << 8 , bar->dimensions.width - 15 + bar->cache ) >> 8 );
	else
		bar->internalSetValue( div32( ( event.getPosY() - deltaY - 8 ) * bar->length2 << 8 , bar->dimensions.height - 15 + bar->cache ) >> 8 );
	
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
				bar->internalSetValue( bar->length2 - bar->length );
			else
				bar->internalSetValue( bar->value + bar->step );
			break;
		case 5:
		case 1:
			if( bar->value <= 0 )
				break;
			if( (int)bar->value - (int)bar->step < 0 )
				bar->internalSetValue( 0 );
			else
				bar->internalSetValue( bar->value - bar->step );
			break;
	}
	
	return handled;
}

_callbackReturn _scrollBar::refreshHandler( _event event ) {
	
	if( event.getGadget()->getType() == _gadgetType::button )
	{
		// Receive Gadget
		_button* that = event.getGadget<_button>();
		
		_bitmapPort bP = that->getBitmapPort();
		
		if( event.hasClippingRects() )
			bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
		else
			bP.normalizeClippingRects();
		
		_length myH = bP.getHeight();
		_length myW = bP.getWidth();
		
		switch( that->style.data ){
			case 5:
			case 4:
			case 2:
			case 1:
				if( that->isPressed() )
					bP.copy( 0 , 0 , &btn_background_pressed  );
				else
					bP.copy( 0 , 0 , &btn_background_released  );
				drawArrow( bP , that->style.data );
				break;
			case 3:
				if( that->isPressed() )
				{
					_bitmap bm = { snip_pressed_bmp , 8 , 1 };
					bP.copyVerticalStretch( 0 , 2 , myH - 4 , &bm );
					
					bP.deleteClippingRects();
					bP.addClippingRects( _rect( 0 , 0 , 8 , 2 ) );
					bP.copy( 0 , 0 , &btn_background_pressed );
					
					bP.deleteClippingRects();
					bP.addClippingRects( _rect( 0 , myH - 2 , 8 , 2 ) );
					bP.copy( 0 , myH - 8 , &btn_background_pressed );
					
					if( myH > 9 )
					{
						bP.normalizeClippingRects();
						if( myH < 12 )
						{
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 2 , 4 , RGB( 25 , 25 , 31 ) );
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 1 , 4 , 60975 );
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) , 4 , RGB( 25 , 25 , 31 ) );
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) + 1 , 4 , 60975 );
						}
						else
						{
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 3 , 4 , RGB( 25 , 25 , 31 ) );
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 2 , 4 , 60975 );
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 1 , 4 , RGB( 25 , 25 , 31 ) );
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) , 4 , 60975 );
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) + 1 , 4 , RGB( 25 , 25 , 31 ) );
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) + 2 , 4 , 60975 );
						}
					}
				}
				else
				{
					_bitmap bm = { snip_released_bmp , 8 , 1 };
					bP.copyVerticalStretch( 0 , 2 , myH - 4 , &bm );
					
					bP.deleteClippingRects();
					bP.addClippingRects( _rect( 0 , 0 , 8 , 2 ) );
					bP.copy( 0 , 0 , &btn_background_released );
					
					bP.deleteClippingRects();
					bP.addClippingRects( _rect( 0 , myH - 2 , 8 , 2 ) );
					bP.copy( 0 , myH - 8 , &btn_background_released );
					
					if( myH > 9 )
					{
						bP.normalizeClippingRects();
						if( myH < 12 )
						{
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 2 , 4 , RGB( 29 , 29 , 31 ) );
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 1 , 4 , 61074 );
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) , 4 , RGB( 29 , 29 , 31 ) );
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) + 1 , 4 , 61074 );
						}
						else
						{
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 3 , 4 , RGB( 29 , 29 , 31 ) );
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 2 , 4 , 61074 );
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 1 , 4 , RGB( 29 , 29 , 31 ) );
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) , 4 , 61074 );
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) + 1 , 4 , RGB( 29 , 29 , 31 ) );
							bP.drawHorizontalLine( 2 , ( myH >> 1 ) + 2 , 4 , 61074 );
						}
					}
				}
				break;
			case 0:
				if( that->isPressed() )
				{
					_bitmap bm = { snip_pressed_bmp , 1 , 8 };
					bP.copyHorizontalStretch( 2 , 0 , myW - 4 , &bm );
					
					bP.deleteClippingRects();
					bP.addClippingRects( _rect( 0 , 0 , 2 , 8 ) );
					bP.copy( 0 , 0 , &btn_background_pressed );
					
					bP.deleteClippingRects();
					bP.addClippingRects( _rect( myW - 2 , 0 , 2 , 8 ) );
					bP.copy( myW - 8 , 0 , &btn_background_pressed );
					
					if( myW > 9 )
					{
						bP.normalizeClippingRects();
						if( myW < 12 )
						{
							bP.drawVerticalLine( ( myW >> 1 ) - 2 , 2 , 4 , RGB( 25 , 25 , 31 ) );
							bP.drawVerticalLine( ( myW >> 1 ) - 1 , 2 , 4 , 60975 );
							bP.drawVerticalLine( ( myW >> 1 ) , 2 , 4 , RGB( 25 , 25 , 31 ) );
							bP.drawVerticalLine( ( myW >> 1 ) + 1 , 2 , 4 , 60975 );
						}
						else
						{
							bP.drawVerticalLine( ( myW >> 1 ) - 3 , 2 , 4 , RGB( 25 , 25 , 31 ) );
							bP.drawVerticalLine( ( myW >> 1 ) - 2 , 2 , 4 , 60975 );
							bP.drawVerticalLine( ( myW >> 1 ) - 1 , 2 , 4 , RGB( 25 , 25 , 31 ) );
							bP.drawVerticalLine( ( myW >> 1 ) , 2 , 4 , 60975 );
							bP.drawVerticalLine( ( myW >> 1 ) + 1 , 2 , 4 , RGB( 25 , 25 , 31 ) );
							bP.drawVerticalLine( ( myW >> 1 ) + 2 , 2 , 4 , 60975 );
						}
					}
				}
				else
				{
					_bitmap bm = { snip_released_bmp , 1 , 8 };
					bP.copyHorizontalStretch( 2 , 0 , myW - 4 , &bm );
					
					bP.deleteClippingRects();
					bP.addClippingRects( _rect( 0 , 0 , 2 , 8 ) );
					bP.copy( 0 , 0 , &btn_background_released );
					
					bP.deleteClippingRects();
					bP.addClippingRects( _rect( myW - 2 , 0 , 2 , 8 ) );
					bP.copy( myW - 8 , 0 , &btn_background_released );
					
					if( myW > 9 )
					{
						bP.normalizeClippingRects();
						if( myW < 12 )
						{
							bP.drawVerticalLine( ( myW >> 1 ) - 2 , 2 , 4 , RGB( 29 , 29 , 31 ) );
							bP.drawVerticalLine( ( myW >> 1 ) - 1 , 2 , 4 , 61074 );
							bP.drawVerticalLine( ( myW >> 1 ) , 2 , 4 , RGB( 29 , 29 , 31 ) );
							bP.drawVerticalLine( ( myW >> 1 ) + 1 , 2 , 4 , 61074 );
						}
						else
						{
							bP.drawVerticalLine( ( myW >> 1 ) - 3 , 2 , 4 , RGB( 29 , 29 , 31 ) );
							bP.drawVerticalLine( ( myW >> 1 ) - 2 , 2 , 4 , 61074 );
							bP.drawVerticalLine( ( myW >> 1 ) - 1 , 2 , 4 , RGB( 29 , 29 , 31 ) );
							bP.drawVerticalLine( ( myW >> 1 ) , 2 , 4 , 61074 );
							bP.drawVerticalLine( ( myW >> 1 ) + 1 , 2 , 4 , RGB( 29 , 29 , 31 ) );
							bP.drawVerticalLine( ( myW >> 1 ) + 2 , 2 , 4 , 61074 );
						}
					}
				}
				break;
			default:
				break;
		}
	}
	else
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
			bP.copyHorizontalStretch( 8 , 0 , that->dimensions.width - 16 , &scroll_bg_vert );
		else
			bP.copyVerticalStretch( 0 , 8 , that->dimensions.height - 16 , &scroll_bg_horiz );
		
	}
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

void _scrollBar::setValue( _u32 value )
{
	if( this->value != value )
	{
		this->value = mid( 0 , (int)value , int( this->length2 - this->length ) );
		refreshPosition();
	}
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