#include "_gadget/gadget.imagegadget.h"

_gadgetEventReturnType _imagegadget::refreshHandler( _gadgetEvent event )
{	
	// Receive Gadget
	_imagegadget* that = event.getGadget<_imagegadget>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	if( that->img )
		bP.copyTransparent( 0 , 0 , that->img );
	
	return use_default;
}

_imagegadget::_imagegadget( _coord x , _coord y , const _bitmap* img , _gadgetStyle style ) :
	_gadget( _gadgetType::imagegadget , img->getWidth() , img->getHeight() , x , y , style ) , img( img )
{
	this->bitmap->reset( NO_COLOR );
	
	// Register Event-Handler
	this->unregisterEventHandler( "mouseDoubleClick" );
	this->registerEventHandler( "refresh" , &_imagegadget::refreshHandler );

	// Refresh Me
	this->refreshBitmap();
}