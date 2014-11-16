#include <_gadget/gadget.checkbox.h>
#include <_type/type.gadget.helpers.h>

// Graphics
#include <_resource/resource.image.checkboxes.h>

_constBitmap _checkBox::graphic[3] = { BMP_checkBoxUnchecked() , BMP_checkBoxChecked() , BMP_checkBoxSemi() };

_callbackReturn _checkBox::refreshHandler( _event event )
{
	// Receive Gadget
	_checkBox* that = event.getGadget<_checkBox>();
	
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



_callbackReturn _checkBox::mouseHandler( _event event )
{
	// Receive Gadget
	_checkBox* that = event.getGadget<_checkBox>();
	
	// Change Value
	that->setIntValue( ! bool( that->getIntValue() ) );
	that->triggerEvent( onEdit );
	
	// Refresh
	that->redraw();
	
	return handled;
}

_checkBox::_checkBox( _optValue<_coord> x , _optValue<_coord> y , _u8 value , _style&& style ) :
	_gadget( _gadgetType::checkbox , x , y , 9 , 9 , style | _style::notResizeable | _style::notDoubleClickable | _style::noTransparentParts )
	, intValue( value )
{
	// Register my handlers
	this->setInternalEventHandler( onDraw , make_callback( &_checkBox::refreshHandler ) );
	this->setInternalEventHandler( onMouseClick , make_callback( &_checkBox::mouseHandler ) );
	this->setInternalEventHandler( onMouseEnter , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( onMouseLeave , _gadgetHelpers::eventForwardRefresh() );
	
	// Refresh Me
	this->redraw();
}