#include "_gadget/gadget.imagegadget.h"

_callbackReturn _imagegadget::refreshHandler( _event event ){
	return handled; // If we would return 'use_default', children would be painted on my bitmap
}

_imagegadget::_imagegadget( _optValue<_coord> x , _optValue<_coord> y , _bitmap&& img , _style&& style ) :
	_gadget( _gadgetType::imagegadget , x , y , img.getWidth() , img.getHeight() , (_bitmap&&)img , (_style&&)style )
{
	// Register Event-Handler
	this->setInternalEventHandler( onDraw , make_callback( &_imagegadget::refreshHandler ) );
}