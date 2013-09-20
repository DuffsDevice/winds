#include "_gadget/gadget.checkbox.h"

// Graphics
#include "_resource/BMP_Checkboxes.h"
#include "_resource/BMP_WindowHeader.h"

const _bitmap _checkbox::graphic[3] = { BMP_CheckboxUnchecked() , BMP_CheckboxChecked() , BMP_CheckboxSemi() };

_callbackReturn _checkbox::refreshHandler( _event event )
{
	// Receive Gadget
	_checkbox* that = event.getGadget<_checkbox>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( NO_COLOR );
	
	//Border
	if( that->isPressed() )
		bP.drawRect( 0 	, 0 , 9 , 9 , RGB( 4 , 11 , 15 ) );
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



_checkbox::_checkbox( _coord x , _coord y , _style&& style ) :
	_gadget( _gadgetType::checkbox , 40 , 9 , x , y , style | _styleAttr::notResizeable )
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