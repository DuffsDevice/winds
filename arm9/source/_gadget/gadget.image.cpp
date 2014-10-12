#include <_gadget/gadget.image.h>

_callbackReturn _imageGadget::refreshHandler( _event event ){
	return handled; // If we would return 'use_default', children would be painted on my bitmap
}

_imageGadget::_imageGadget( _optValue<_coord> x , _optValue<_coord> y , _bitmap img , _optValue<_length> width , _optValue<_length> height , _style&& style ) :
	_gadget( _gadgetType::imagegadget , x , y , width.isValid() ? _length(width) : img.getWidth() , height.isValid() ? _length(height) : img.getHeight() , (_bitmap&&)img , (_style&&)style )
{
	// Register Event-Handler
	this->setInternalEventHandler( onDraw , make_callback( &_imageGadget::refreshHandler ) );
}