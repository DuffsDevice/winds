#include <_gadget/gadget.colorpicker.hexagon.h>
#include <_resource/resource.image.picker.color.h>
#include <_resource/resource.image.picker.greyscale.h>
#include <_resource/resource.image.picker.overlay.h>
#include <_resource/resource.image.picker.overlay.big.h>
#include <_controller/controller.gui.h>

#define COLORPICKER_BW_START_Y 95
#define COLORPICKER_NUM_HEXAGONS 145

const _length hexagonWidth				= 8;
const _length hexagonHeight				= 7;
const _coord hexagonStartX[13]			= { 25 , 21 , 17 , 13 , 9 , 5 , 1 , 5 , 9 , 13 , 17 , 21 , 25 };
const _coord hexagonNumberPerLine[13]	= { 7 , 8 , 9 , 10 , 11 , 12 , 13 , 12 , 11 , 10 , 9 , 8 , 7 };

_size _hexagonColorPicker::getHexagonSize( _s16 number )
{
	if( number == 127 || number == 144 )
		return _size( 15 , 16 ); // White or Black
	return _size( hexagonWidth , hexagonHeight );
}

_pos _hexagonColorPicker::getHexagonPos( _s16 number )
{
	if( number < 0 )
		return _pos(0);
	
	for( _u8 curRow = 0 ; curRow < 13 ; curRow++ )
	{
		if( number < hexagonNumberPerLine[curRow] )
			return _pos( 1 + hexagonStartX[curRow] + number * hexagonWidth , 1 + hexagonHeight * curRow );
		number -= hexagonNumberPerLine[curRow];
	}
	if( number == 0 )
		return _pos( 1 , COLORPICKER_BW_START_Y ); // White
	if( number >= 17 )
		return _pos( 89 , COLORPICKER_BW_START_Y ); // Black
	return _pos( 15 + 4 * number , COLORPICKER_BW_START_Y + ( (number+1) & 1 ) * 7 );
}

_color _hexagonColorPicker::getHexagonColor( _s16 number ){
	if( number < 0 )
		return _color::transparent;
	
	_pos pos = getHexagonPos( number );
	return BMP_ColorPicker().getPixel( pos.first + 3 , pos.second + 1 );
}

_hexagonColorPicker::_hexagonColorPicker( _optValue<_coord> x , _optValue<_coord> y , _color initialColor , _style&& style ) :
	_gadget( _gadgetType::hexagoncolorpicker , x , y , 106 , COLORPICKER_BW_START_Y + 16 , style | _style::notResizeable | _style::draggable )
	, bgColor( _guiController::getControlBg() )
{
	// Set Hexagon
	setColor( _color::fromRGB( 31 , 15 , 24 ) );
	
	this->setInternalEventHandler( onDragging , make_callback( this , &_hexagonColorPicker::inputHandler ) );
	this->setInternalEventHandler( onKeyDown , make_callback( this , &_hexagonColorPicker::inputHandler ) );
	this->setInternalEventHandler( onKeyRepeat , make_callback( this , &_hexagonColorPicker::inputHandler ) );
	this->setInternalEventHandler( onMouseDown , make_callback( this , &_hexagonColorPicker::inputHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( this , &_hexagonColorPicker::refreshHandler ) );
	
	// Redraw me!
	this->redraw();
}

void _hexagonColorPicker::setColor( _color color )
{
	_s8 bestHex = -1;
	_u16 distance = ( 1 << 16 ) - 1;
	_u8 actualNumber = 0;
	
	// Determine the hexagon with the smallest color distance to the given color
	for( _u16 curHex = 0 ; curHex < COLORPICKER_NUM_HEXAGONS ; curHex++ )
	{
		_u16 newDistance = _color::distance( _hexagonColorPicker::getHexagonColor( curHex ) , color , true );
		
		if( newDistance < distance ){
			distance = newDistance;
			bestHex = actualNumber;
		}
	}
	if( bestHex >= 0 ){
		this->hexagonNumber = bestHex;
		this->color = color;
		this->redraw();
	}
}

_callbackReturn _hexagonColorPicker::refreshHandler( _event event )
{
	// Fetch Gadget
	_hexagonColorPicker* that = event.getGadget<_hexagonColorPicker>();
	
	// get bitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	// Reset to white
	bP.fill( that->bgColor );
	
	// Copy Images
	bP.copyTransparent( 1 , 1 , BMP_ColorPicker() );
	bP.copyTransparent( 1 , COLORPICKER_BW_START_Y , BMP_ColorPickerGreyscale() );
	
	if( that->hexagonNumber >= 0 ){
		_pos position = _hexagonColorPicker::getHexagonPos( that->hexagonNumber );
		if( _hexagonColorPicker::getHexagonSize( that->hexagonNumber ).first > hexagonWidth )
			bP.copyTransparent( position.first , position.second , BMP_ColorPickerOverlayBig() );
		else
			bP.copyTransparent( position.first-1 , position.second-1 , BMP_ColorPickerOverlay() );
	}
	
	return use_default;
}

_callbackReturn _hexagonColorPicker::inputHandler( _event event )
{
	_hexagonColorPicker* that = event.getGadget<_hexagonColorPicker>();
	
	_coord posX = event.getPosX();
	_coord posY = event.getPosY();
	
	_s16 oldHexNumber = that->hexagonNumber;
	_color oldColor = that->color;
	that->hexagonNumber = -1;
	
	for( _u16 curHex = 0 ; curHex < COLORPICKER_NUM_HEXAGONS ; curHex++ )
	{
		_rect rect = _rect( _hexagonColorPicker::getHexagonPos( curHex ) , _hexagonColorPicker::getHexagonSize( curHex ) );
		
		// Check if the mouse is inside
		if( rect.contains( posX , posY ) ){
			that->color = _hexagonColorPicker::getHexagonColor( curHex ); // Determine Color
			that->hexagonNumber = curHex; // Set active hexagon
			break;
		}
	}
	
	// Check if the color value of the colorpicker has changed -> throw event
	if( that->color != oldColor )
		that->triggerEvent( onEdit );
	
	// Check if something visually has changed
	if( oldHexNumber != that->hexagonNumber )
		that->redraw();
	
	return handled;
}