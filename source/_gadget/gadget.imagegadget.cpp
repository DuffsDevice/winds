#include "_gadget/gadget.imagegadget.h"

_gadgetEventReturnType _imagegadget::refreshHandler( _gadgetEvent event )
{	
	// Receive Gadget
	_imagegadget* that = (_imagegadget*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().isBubbleRefresh() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	if( that->img )
		bP.copyTransparent( 0 , 0 , that->img );
	
	return use_default;
}

_imagegadget::_imagegadget( _coord x , _coord y , const _bitmap* img , _gadgetStyle style ) :
	_gadget( _gadgetType::imagegadget , img->getWidth() , img->getHeight() , x , y , style ) , img( img )
{
	this->bitmap->reset( NO_COLOR );
	
	// Register Event-Handler
	this->unregisterEventHandler( mouseDoubleClick );
	this->registerEventHandler( refresh , &_imagegadget::refreshHandler );

	// Refresh Me
	this->refreshBitmap();
}