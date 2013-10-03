#include "_gadget/gadget.window.button.h"
#include "_gadget/gadget.window.h"

_callbackReturn _windowButton::refreshHandler( _event event )
{
	// Get Source
	_windowButton* that = event.getGadget<_windowButton>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_pixel color[] = { 
		RGB255( 114 , 154 , 250 ) , RGB255( 5 , 88 , 226 ) , RGB255( 75 , 126 , 245 ) , // Blue Values
		RGB255( 227 , 92 , 59 ) , RGB255( 234 , 131 , 106 ) , RGB255( 250 , 221 , 211 ) , // Red Values
		RGB255( 186 , 61 , 239 ) // Violet (Blue =/= Red)
	};
	
	bP.fill( NO_COLOR );
	
	switch( that->buttonType ){
		case 0:
			// Close
			bP.drawRect( 0 , 0 , 8 , 8 , color[3] );
			bP.drawRect( 1 , 1 , 6 , 6 , color[4] );
			bP.drawRect( 2 , 2 , 4 , 4 , color[4] );
			bP.drawFilledRect( 3 , 3 , 2 , 2 , color[5] );
			bP.drawPixel( 2 , 2 , color[5] );
			bP.drawPixel( 2 , 5 , color[5] );
			bP.drawPixel( 5 , 2 , color[5] );
			bP.drawPixel( 5 , 5 , color[5] );
			break;
		case 1:
			// Maximize/Restore
			bP.drawRect( 0 , 0 , 8 , 8 , color[0] ); // White Border
			
			if( ((_window*)that->getParent())->isMaximized() )
			{
				bP.drawRect( 3 , 2 , 3 , 3 , color[0] ); // Semi-White Line
				bP.drawRect( 2 , 4 , 2 , 2 , color[0] ); // Semi-White Line
				bP.drawHorizontalLine( 3 , 2 , 3 , COLOR_WHITE ); // White Topper Line
				bP.drawHorizontalLine( 2 , 4 , 2 , COLOR_WHITE ); // White Topper Line
			}
			else
			{
				bP.drawRect( 2 , 2 , 4 , 4 , color[0] ); // Semi-White Line
				bP.drawHorizontalLine( 2 , 2 , 4 , COLOR_WHITE ); // White Topper Line
			}
			break;
		case 2:
			// Minimize
			bP.drawRect( 0 , 0 , 8 , 8 , color[0] );
			bP.drawHorizontalLine( 2 , 5 , 3 , RGB( 31 , 31 , 31 ) );
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

_windowButton::_windowButton( _coord x , _coord y , _u8 buttonType , _style&& style ) :
	_button( 8 , 8 , x , y , "" , (_style&&)style )
	, buttonType( buttonType )
{
	// Reset Bitmap
	this->bitmap.reset( COLOR_RED );
	
	this->setInternalEventHandler( onDraw , make_callback( &_windowButton::refreshHandler ) );
	this->redraw();
}