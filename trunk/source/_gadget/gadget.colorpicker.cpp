#include "_gadget/gadget.colorpicker.h"

#include "_type/type.color.h"
#include "_type/type.system.h"
#include "_type/type.font.glyphs.h"

#include <math.h>

#define M_PI 3.14159265359f

_colorpicker::_colorpicker( _length width , _length height , _coord x , _coord y , _pixel initialColor , _style style ) :
	_gadget( _gadgetType::colorpicker , width , height , x , y , style )
	, hueSatImage( width - 14 , height - 2 )
{
	// Set Color
	_color c = initialColor;
	this->lum = c.getL();
	this->sat = c.getS();
	this->hue = c.getH();
	
	refreshBigGradient();
	
	this->hueSatTable = new _gadget( hueSatImage.getWidth() , hueSatImage.getHeight() , 1 , 1 , _styleAttr() | _styleAttr::smallDragTrig | _styleAttr::draggable );
	this->lumTable = new _gadget( 11 , hueSatImage.getHeight() , hueSatImage.getWidth() + 2 , 1 , _styleAttr() | _styleAttr::smallDragTrig | _styleAttr::draggable );
	
	this->registerEventHandler( refresh , new _classCallback( this , &_colorpicker::refreshHandler ) );
	this->registerEventHandler( onResize , new _classCallback( this , &_colorpicker::refreshHandler ) );
	
	this->hueSatTable->registerEventHandler( dragging , new _classCallback( this , &_colorpicker::inputHandler ) );
	this->hueSatTable->registerEventHandler( keyDown , new _classCallback( this , &_colorpicker::inputHandler ) );
	this->hueSatTable->registerEventHandler( keyRepeat , new _classCallback( this , &_colorpicker::inputHandler ) );
	this->hueSatTable->registerEventHandler( mouseDown , new _classCallback( this , &_colorpicker::inputHandler ) );
	this->hueSatTable->registerEventHandler( refresh , new _classCallback( this , &_colorpicker::refreshHandler ) );
	this->hueSatTable->refreshBitmap();
	
	this->lumTable->registerEventHandler( dragging , new _classCallback( this , &_colorpicker::inputHandler ) );
	this->lumTable->registerEventHandler( mouseDown , new _classCallback( this , &_colorpicker::inputHandler ) );
	this->lumTable->registerEventHandler( keyRepeat , new _classCallback( this , &_colorpicker::inputHandler ) );
	this->lumTable->registerEventHandler( keyDown , new _classCallback( this , &_colorpicker::inputHandler ) );
	this->lumTable->registerEventHandler( refresh , new _classCallback( this , &_colorpicker::refreshHandler ) );
	this->lumTable->refreshBitmap();
	
	// Add Image
	this->addChild( this->hueSatTable );
	this->addChild( this->lumTable );
}

_pixel _colorpicker::getSelectedColor() const
{
	return _color()
		.setHSL( this->hue , this->sat , this->lum )
		.getColor();
}

void _colorpicker::selectColor( _pixel color )
{
	_color c = color;
	
	_int newHue = c.getH();
	_int newSat = c.getS();
	_int newLum = c.getL();
	
	if( this->hue != newHue || this->sat != newSat )
	{
		this->hue = newHue;
		this->sat = newSat;
		this->lum = newLum;
		this->hueSatTable->bubbleRefresh( true );
		this->lumTable->bubbleRefresh( true );
	}
	else if( this->lum != newLum )
	{
		this->lum = newLum;
		this->lumTable->bubbleRefresh( true );
	}
}

_callbackReturn _colorpicker::refreshHandler( _event event )
{

	if( event.getGadget() == this->hueSatTable )
	{
		_bitmapPort bP = event.getGadget()->getBitmapPort();
		
		if( event.hasClippingRects() )
			bP.addClippingRects( event.getDamagedRects().toRelative( event.getGadget()->getAbsolutePosition() ) );
		else
			bP.normalizeClippingRects();
		
		bP.copy( 0 , 0 , this->hueSatImage );
		
		// Compute X/Y - Coordinates
		_int x = this->hue * this->hueSatTable->getWidth();
		x /= 361;
		_int y = this->sat * this->hueSatTable->getHeight() + 50;
		y /= 101;
		
		bP.drawChar( x - 2 , this->hueSatTable->getHeight() - y - 5 , _system::getFont("SystemSymbols8") , glyph::circle , COLOR_BLACK );
	}
	else if( event.getGadget() == this->lumTable )
	{
		_bitmapPort bP = event.getGadget()->getBitmapPort();
		
		if( event.hasClippingRects() )
			bP.addClippingRects( event.getDamagedRects().toRelative( event.getGadget()->getAbsolutePosition() ) );
		else
			bP.normalizeClippingRects();
		
		// Compute Y - Coordinate
		_int y = ( 100 - this->lum ) * this->lumTable->getHeight() + 50;
		y /= 101;
		
		bP.fill( COLOR_WHITE );
		
		// Draw Gradient!
		_int height = event.getGadget()->getHeight();
		_pixel col = _color().setHSL( this->hue , this->sat , 50 ).getColor();
		bP.drawVerticalGradient( 0 , 0 , 7 , height >> 1 , COLOR_WHITE , col );
		bP.drawVerticalGradient( 0 , height >> 1 , 7 , height >> 1 , col , COLOR_BLACK );
		
		// Draw Arrow
		bP.drawChar( 8 , y - 4 , _system::getFont("SystemSymbols8") , glyph::arrowLeft , COLOR_BLACK );
	}
	else
	{
		// Resize event?
		if( event.getType() == onResize )
		{
			// Refresh the colorful Gradient
			this->hueSatImage.resize( this->getWidth() - 14 , this->getHeight() - 2 );
			this->refreshBigGradient();
			
			// Resize both gradients
			this->hueSatTable->setDimensions( _rect( 1 , 1 , this->getWidth() - 14 , this->getHeight() - 2 ) );
			this->lumTable->setDimensions( _rect( hueSatImage.getWidth() + 2 , 1 , 11 , hueSatImage.getHeight() ) );
			
			return handled;
		}
		
		_bitmapPort bP = this->getBitmapPort();
		
		if( event.hasClippingRects() )
			bP.addClippingRects( event.getDamagedRects().toRelative( this->getAbsolutePosition() ) );
		else
			bP.normalizeClippingRects();
		
		bP.fill( COLOR_WHITE );
	}
	
	return handled;
}

void _colorpicker::refreshBigGradient()
{
	// Paint Color Table
	_length imgWidth = this->hueSatImage.getWidth();
	_length imgHeight = this->hueSatImage.getHeight();
	
	for( int x = 0 ; x < imgWidth ; x++ )
		this->hueSatImage.drawVerticalGradient( x , 0 , 1 , imgHeight , _color().setHSL( x * 360 / imgWidth , 100 , 50 ).getColor() , COLOR_GRAY );
	
	// Paint Color Table
	//_length imgWidth = this->hueSatImage.getWidth();
	//_length imgHeight = this->hueSatImage.getHeight();
	//
	//_length radius = min( imgWidth >> 1 , imgHeight >> 1 );
	//_length radiusSmall = radius - 8;
	//
	//for( int a = 0 ; a < 360 ; a++ )
	//{
	//	float radiants = float( a ) * M_PI / 180;
	//	float c = cos( radiants );
	//	float s = sin( radiants );
	//	_2s32 outer = _2s32( imgWidth / 2 + c * radius , imgHeight / 2 + s * radius );
	//	_2s32 inner = _2s32( imgWidth / 2 + c * radiusSmall , imgHeight / 2 + s * radiusSmall );
	//	this->hueSatImage.drawLine( outer.first , outer.second , inner.first , inner.second , _color().setHSL( a , 100 , 50 ).getColor() );
	//}
}

_callbackReturn _colorpicker::inputHandler( _event event )
{
	_gadget* hSTable = this->hueSatTable;
	_gadget* lTable = this->lumTable;
	
	_int newHue = this->hue;
	_int newSat = this->sat;
	_int newLum = this->lum;
	
	if( event.getGadget() == hSTable )
	{
		if( event.getType() == keyDown || event.getType() == keyRepeat )
		{
			_s8 dx = ( event.getKeyCode() == DSWindows::KEY_RIGHT ) - ( event.getKeyCode() == DSWindows::KEY_LEFT );
			_s8 dy = ( event.getKeyCode() == DSWindows::KEY_UP ) - ( event.getKeyCode() == DSWindows::KEY_DOWN );
			newHue = ( this->hue + dx * 3 + 360 ) % 360;
			newSat = mid( 0 , this->sat + dy * 3 , 100 );
			
			goto end;
		}
		else if( event.getType() == dragging )
		{
			event.setPosX( event.getPosX() - hSTable->getX() );
			event.setPosY( event.getPosY() - hSTable->getY() );
		}
		
		_int x = event.getPosX() * 360 + ( hSTable->getWidth() >> 1 );
		_int y = hSTable->getHeight() - event.getPosY() - 1;
		y *= 100;
		y += hSTable->getHeight() >> 1;
		
		newHue = mid( 0 , x / ( hSTable->getWidth() - 1 ) , 360 );
		newSat = mid( 0 , y / ( hSTable->getHeight() - 1 ) , 100 );
	}
	else if( event.getGadget() == lTable )
	{
		if( event.getType() == keyDown || event.getType() == keyRepeat )
		{
			_s8 dy = ( event.getKeyCode() == DSWindows::KEY_UP ) - ( event.getKeyCode() == DSWindows::KEY_DOWN );
			newLum = mid( 0 , this->lum + dy * 3 , 100 );
			
			goto end;
		}
		else if( event.getType() == dragging )
			event.setPosY( event.getPosY() - lTable->getY() );
		
		_int y = max( 0 , min( lTable->getHeight() - 1 - event.getPosY() , lTable->getHeight() - 1 ) );
		y *= 100;
		y += lTable->getHeight() >> 1;
		
		newLum = mid( 0 , y / ( lTable->getHeight() - 1 ) , 100 );
	}
	
	end:
	
	if( this->hue != newHue || this->sat != newSat )
	{
		this->hue = newHue;
		this->sat = newSat;
		this->lum = newLum;
		hSTable->bubbleRefresh( true );
		lTable->bubbleRefresh( true );
		
		this->triggerEvent( onChange );
	}
	else if( this->lum != newLum )
	{
		this->lum = newLum;
		lTable->bubbleRefresh( true );
		
		this->triggerEvent( onChange );
	}
	
	return handled;
}

_colorpicker::~_colorpicker()
{
	this->removeChildren( true );
}