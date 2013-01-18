#include "_gadget/gadget.imagegadget.h"

void _imagegadget::setImage( const _bitmap& img )
{
	this->img = img;
	this->setHeight( img.getHeight() );
	this->setWidth( img.getWidth() );
	this->bubbleRefresh( true );
}

_callbackReturn _imagegadget::refreshHandler( _event event )
{	
	// Receive Gadget
	_imagegadget* that = event.getGadget<_imagegadget>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().relativate( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	if( that->img.isValid() )
		bP.copyTransparent( 0 , 0 , that->img );
	
	return use_default;
}

_imagegadget::_imagegadget( _coord x , _coord y , const _bitmap& img , _style style ) :
	_gadget( _gadgetType::imagegadget , img.getWidth() , img.getHeight() , x , y , style ) , img( img )
{
	// Register Event-Handler
	this->unregisterEventHandler( mouseDoubleClick );
	this->registerEventHandler( refresh , new _staticCallback( &_imagegadget::refreshHandler ) );
	
	this->bitmap.reset( NO_COLOR );

	// Refresh Me
	this->refreshBitmap();
}