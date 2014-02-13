#include "_gadget/gadget.checkbox.h"

// Graphics
#include "_resource/resource.image.checkboxes.h"

_constBitmap _checkbox::graphic[3] = { BMP_CheckboxUnchecked() , BMP_CheckboxChecked() , BMP_CheckboxSemi() };

_callbackReturn _checkbox::refreshHandler( _event event )
{
	// Receive Gadget
	_checkbox* that = event.getGadget<_checkbox>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( _color::transparent );
	
	//Border
	if( that->isPressed() )
		bP.drawRect( 0 	, 0 , 9 , 9 , _color::fromRGB( 4 , 11 , 15 ) );
	else
		bP.drawRect( 0 	, 0 , 9 , 9 , 54829 );
	
	// Content
	bP.copy( 1 , 1 , that->graphic[ mid( 0 , that->getIntValue() , 2 ) ]);
	
	return use_default;
}



_callbackReturn _checkbox::mouseHandler( _event event )
{
	// Receive Gadget
	_checkbox* that = event.getGadget<_checkbox>();
	
	// Change Value
	that->setIntValue( ! bool( that->getIntValue() ) );
	that->triggerEvent( onEdit );
	
	// Refresh
	that->redraw();
	
	return handled;
}



_checkbox::_checkbox( _optValue<_coord> x , _optValue<_coord> y , _style&& style ) :
	_gadget( _gadgetType::checkbox , x , y , 9 , 9 , style | _styleAttr::notResizeable )
	, intValue( 0 )
{
	// Register my handlers
	this->setInternalEventHandler( onDraw , make_callback( &_checkbox::refreshHandler ) );
	this->setInternalEventHandler( onMouseClick , make_callback( &_checkbox::mouseHandler ) );
	this->setInternalEventHandler( onMouseEnter , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( onMouseLeave , _gadgetHelpers::eventForwardRefresh() );
	
	// Refresh Me
	this->redraw();
}