#include "_gadget/gadget.window.button.h"
#include "_gadget/gadget.window.h"

_callbackReturn _windowButton::refreshHandler( _event event )
{
	// Get Source
	_windowButton* that = event.getGadget<_windowButton>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_color color[] = { 
		_color::fromRGB8( 114 , 154 , 250 ) , _color::fromRGB8( 5 , 88 , 226 ) , _color::fromRGB8( 75 , 126 , 245 ) , // Blue Values
		_color::fromRGB8( 235 , 65 , 46 ) , _color::fromRGB8( 248 , 116 , 88 ) , _color::fromRGB8( 250 , 221 , 211 ) , // Red Values
		_color::fromRGB8( 186 , 61 , 239 ) , // Violet (Blue <-> Red)
		_color::fromRGB8( 215 , 11 , 0 ) // Dark Red (Pressed)
	};
	
	bP.fill( _color::transparent );
	
	switch( that->buttonType ){
		case 0:
		{
			// Close
			bP.drawRect( 0 , 0 , 8 , 8 , color[3] );
			_u8 index = that->isPressed() ? 7 : 4;
			bP.drawRect( 1 , 1 , 6 , 6 , color[index] );
			bP.drawRect( 2 , 2 , 4 , 4 , color[index] );
			bP.drawFilledRect( 3 , 3 , 2 , 2 , color[5] );
			bP.drawPixel( 2 , 2 , color[5] );
			bP.drawPixel( 2 , 5 , color[5] );
			bP.drawPixel( 5 , 2 , color[5] );
			bP.drawPixel( 5 , 5 , color[5] );
			break;
		}
		case 1:
			if( that->isPressed() )
				bP.fill( _color::fromRGB8( 24 , 65 , 158 ) );
			
			// Maximize/Restore
			bP.drawRect( 0 , 0 , 8 , 8 , color[0] ); // White Border
			
			if( ((_window*)that->getParent())->isMaximized() )
			{
				bP.drawRect( 3 , 2 , 3 , 3 , color[0] ); // Semi-White Line
				bP.drawRect( 2 , 4 , 2 , 2 , color[0] ); // Semi-White Line
				bP.drawHorizontalLine( 3 , 2 , 3 , _color::white ); // White Topper Line
				bP.drawHorizontalLine( 2 , 4 , 2 , _color::white ); // White Topper Line
			}
			else
			{
				bP.drawRect( 2 , 2 , 4 , 4 , color[0] ); // Semi-White Line
				bP.drawHorizontalLine( 2 , 2 , 4 , _color::white ); // White Topper Line
			}
			break;
		case 2:
			if( that->isPressed() )
				bP.fill( _color::fromRGB8( 24 , 65 , 158 ) );
			
			// Minimize
			bP.drawRect( 0 , 0 , 8 , 8 , color[0] );
			bP.drawHorizontalLine( 2 , 5 , 3 , _color::fromRGB( 31 , 31 , 31 ) );
			break;
	}
	
	// Not the close button
	if( that->buttonType > 0 ){
		bP.drawPixel( 0 , 0 , color[2] );
		bP.drawPixel( 7 , 0 , color[2] );
		bP.drawPixel( 7 , 7 , color[2] );
		bP.drawPixel( 0 , 7 , color[2] );
	}
	else{
		bP.drawPixel( 0 , 0 , color[6] );
		bP.drawPixel( 7 , 0 , color[6] );
		bP.drawPixel( 7 , 7 , color[6] );
		bP.drawPixel( 0 , 7 , color[6] );
	}
	
	return use_default;
}

_windowButton::_windowButton( _optValue<_coord> x , _optValue<_coord> y , _u8 buttonType , _style&& style ) :
	_button( x , y , 8 , 8 , "" , (_style&&)style )
	, buttonType( buttonType )
{
	// Reset Bitmap
	this->bitmap.reset( _color::red );
	
	this->setInternalEventHandler( onDraw , make_callback( &_windowButton::refreshHandler ) );
	this->redraw();
}