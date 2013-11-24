#include "_gadget/gadget.bootupScreen.h"
#include "_resource/BMP_WindowsBootLogo.h"

_constbitmap _bootupScreen::bootLogo = BMP_WindowsBootLogo();

_callbackReturn _bootupScreen::refreshHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( COLOR_BLACK );
	bP.copy( 78 , 35 , _bootupScreen::bootLogo );
	
	return use_default;
}

_bootupScreen::_bootupScreen( _u8 bgId , _style&& style ) :
	_gadgetScreen( bgId , _gadgetScreenType::bootUp , style | _styleAttr::canNotReceiveFocus)
	, refresher( new _gadget( _gadgetType::none , 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT ) )
	, bar( new _progressbar( 102 , 125 , 51 , false ) )
{	
	refresher->setInternalEventHandler( onDraw , make_callback( &_bootupScreen::refreshHandler ) );
	refresher->redraw();
	this->addChild( refresher );
	
	bar->setColorScheme( true );
	this->addChild( bar );
	
	//! Refresh me
	this->redraw();
}

_bootupScreen::~_bootupScreen()
{
	delete this->refresher;
	delete this->bar;
}