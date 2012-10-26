#include "_gadget/gadget.bootupScreen.h"
#include "_resource/BMP_WindowsBootLogo.h"

_bitmap* bootLogo = new BMP_WindowsBootLogo();

_gadgetEventReturnType _bootupScreen::refreshHandler( _gadgetEvent event )
{
	_gadget* that = event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.resetClippingRects();
	
	bP.fill( COLOR_BLACK );
	
	bP.copy( 78 , 35 , bootLogo );
	
	return use_default;
}

_bootupScreen::_bootupScreen( _u8 bgId , _gadgetStyle style ) :
	_gadgetScreen( bgId , style )
	, refresher( new _gadget( SCREEN_WIDTH , SCREEN_HEIGHT , 0 , 0 ) )
	, bar( new _progressbar( 51 , 102 , 125 , false ) )
{	
	refresher->style.canReceiveFocus = false;
	refresher->registerEventHandler( "refresh" , &_bootupScreen::refreshHandler );
	refresher->refreshBitmap();
	this->addChild( refresher );
	
	refresher->style.canReceiveFocus = false;
	bar->setCol( true );
	this->addChild( bar );
	
	//! Refresh me
	this->refreshBitmap();
}

_bootupScreen::~_bootupScreen()
{
	delete this->refresher;
	delete this->bar;
}