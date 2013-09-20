#include "_gadget/gadget.imagegadget.h"

_callbackReturn _imagegadget::refreshHandler( _event event ){
	return handled; // If we would return 'use_default', children would be painted on my bitmap
}

_imagegadget::_imagegadget( _coord x , _coord y , _bitmap&& img , _style&& style ) :
	_gadget( _gadgetType::imagegadget , img.getWidth() , img.getHeight() , x , y , (_bitmap&&)img , (_style&&)style )
{
	// Register Event-Handler
	this->setInternalEventHandler( onDraw , make_callback( &_imagegadget::refreshHandler ) );
}