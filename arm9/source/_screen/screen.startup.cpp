#include "_screen/screen.startup.h"
#include "_gadget/gadget.image.h"
#include "_gadget/gadget.counter.h"
#include "_gadget/gadget.fileview.h"
#include "_resource/resource.image.windows.bootlogo.h"

_color SUpalette[] = { 
	_color::fromRGB( 0 , 6 , 19 ) , _color::fromRGB( 0 , 6 , 15 ) , _color::fromRGB( 21 , 24 , 31 ) , // --- Topper Bar ( Main color , dark Main , bright Accent )
	_color::fromHex( 0x5A7EDC ) , // Bright Background
	_color::fromHex( 0x3733ac ) , _color::fromHex( 0x0f319f ) , _color::fromHex( 0xd9946b ) , // Bottom Bar ( Main - left side , right side , orange accent )
	_color::fromHex( 0x7193EB )
};

_u8 accntPos = 110;

_callbackReturn _startupScreen::refreshHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( SUpalette[3] );
	bP.drawHorizontalGradient( 0 , 0 , 50 , SCREEN_HEIGHT , SUpalette[7] , SUpalette[3] );
	
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

_startupScreen::_startupScreen( _u8 bgId , _style&& style ) :
	_gadgetScreen( bgId , _gadgetScreenType::startUp , SCREEN_WIDTH , SCREEN_HEIGHT , (_style&&)style )
	, winLogoGadget( new _imageGadget( 4 , 3 , _startupScreen::winLogo , ignore , ignore , _style::notFocusable ) )
{	
	this->setInternalEventHandler( onDraw , make_callback( &_startupScreen::refreshHandler ) );
	this->addChild( winLogoGadget );
	
	// Refresh me
	this->redraw();
}

_constBitmap _startupScreen::winLogo = BMP_WindowsBootLogoSmall();