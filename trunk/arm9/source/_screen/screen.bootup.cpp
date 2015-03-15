#include <_screen/screen.bootup.h>
#include <_resource/resource.image.windows.bootlogo.h>

_callbackReturn _bootupScreen::refreshHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( _color::black );
	bP.copy( 78 , 35 , BMP_WindowsBootLogo() );
	
	return use_default;
}

_bootupScreen::_bootupScreen( _u8 bgId , _style&& style ) :
	_gadgetScreen( bgId , _gadgetScreenType::bootUp , SCREEN_WIDTH , SCREEN_HEIGHT , style | _style::notFocusable )
	, bar( new _progressBar( 102 , 125 , 51 , false ) )
{
	this->setInternalEventHandler( onDraw , make_callback( &_bootupScreen::refreshHandler ) );
	
	bar->setColorScheme( true );
	this->addChild( bar );
	
	//! Refresh me
	this->redraw();
}

_bootupScreen::~_bootupScreen()
{
	delete this->bar;
}