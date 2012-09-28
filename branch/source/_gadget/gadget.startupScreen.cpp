#include "_gadget/gadget.startupScreen.h"

#include <nds/arm9/console.h>

_pixel SUpalette[] = { 
	RGB( 0 , 6 , 19 ) , RGB( 0 , 6 , 15 ) , RGB( 21 , 24 , 31 ) , // --- Topper Bar ( Main color , dark Main , bright Accent )
	RGBHEX( 0x5A7EDC ) , // Bright Background
	RGBHEX( 0x3733ac ) , RGBHEX( 0x0f319f ) , RGBHEX( 0xd9946b ) // Bottom Bar ( Main - left side , right side , orange accent )
};

_u8 accntPos = 110;

_gadgetEventReturnType _startupScreen::refreshHandler( _gadgetEvent event )
{
	_startupScreen* that = (_startupScreen*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	bP.fill( SUpalette[3] );
	
	// ---------------- The Topper Dark Bar
	bP.drawFilledRect( 0 , 0 , SCREEN_WIDTH , 17 , SUpalette[0] );
	
	bP.drawHorizontalGradient( 0 , 17 , accntPos , 1 , SUpalette[0] , SUpalette[1] );
	bP.drawHorizontalGradient( accntPos , 17 , SCREEN_WIDTH - accntPos , 1 , SUpalette[1] , SUpalette[0] );
	
	bP.drawHorizontalLine( 0 , 18 , SCREEN_WIDTH , SUpalette[0] );
	
	bP.drawHorizontalGradient( 0 , 19 , accntPos , 1 , SUpalette[3] , SUpalette[2] );
	bP.drawHorizontalGradient( accntPos , 19 , SCREEN_WIDTH - accntPos , 1 , SUpalette[2] , SUpalette[3] );
	// ----------------
	
	// ---------------- The Bottom Less Dark Bar
	bP.drawHorizontalGradient( 0 , SCREEN_HEIGHT - 23 , SCREEN_WIDTH , 23 , SUpalette[4] , SUpalette[5] );
	
	bP.drawHorizontalGradient( 0 , SCREEN_HEIGHT - 24 , accntPos , 1 , SUpalette[5] , SUpalette[6] );
	bP.drawHorizontalGradient( accntPos , SCREEN_HEIGHT - 24 , SCREEN_WIDTH - accntPos , 1 , SUpalette[6] , SUpalette[5] );
	// ----------------
	
	return use_default;
}

_startupScreen::_startupScreen( _u8 bgId , _gadgetStyle style ) :
	_gadgetScreen( bgId , style )
{	
	this->registerEventHandler( "refresh" , &_startupScreen::refreshHandler );
	
	//! Refresh me
	this->refreshBitmap();
}