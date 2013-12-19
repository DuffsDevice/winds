#include "_gadget/gadget.slider.h"
#include "_resource/BMP_HorizontalSlider.h"
#include "_resource/BMP_VerticalSlider.h"

_sliderHandle::_sliderHandle( _optValue<_coord> x , _optValue<_coord> y , _dimension dim , _style&& style ) :
	_gadget( _gadgetType::none , x , y , dim == _dimension::horizontal ? 6 : 12 , dim == _dimension::horizontal ? 11 : 6 , style | _styleAttr::draggable | _styleAttr::smallDragTrig )
	, dimension( dim )
{
	// Register Handler
	this->setInternalEventHandler( onDraw , make_callback( &_sliderHandle::refreshHandler ) );
	
	// Refresh
	this->redraw();
}

_callbackReturn _sliderHandle::refreshHandler( _event event )
{
	_sliderHandle* that = event.getGadget<_sliderHandle>();
	
	_bitmapPort bP = that->getBitmapPort( event );
	
	if( that->dimension == _dimension::horizontal )
		bP.copy( 0 , 0 , BMP_HorizontalSlider() );
	else
		bP.copy( 0 , 0 , BMP_VerticalSlider() );
	
	return handled;
}