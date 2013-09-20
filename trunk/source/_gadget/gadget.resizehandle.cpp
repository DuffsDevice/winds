#include "_gadget/gadget.resizehandle.h"

_callbackReturn _resizeHandle::refreshHandler( _event event )
{
	// Receive Gadget
	_resizeHandle* that = event.getGadget<_resizeHandle>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( NO_COLOR );
	
	if( that->parent )
	{
		if( that->parent->isResizeableX() && that->parent->isResizeableY() )
		{
			bP.drawLine( 0 , 7 , 7 , 0 , COLOR_GRAY );
			bP.drawLine( 3 , 7 , 7 , 3 , COLOR_GRAY );
			bP.drawLine( 6 , 7 , 7 , 6 , COLOR_GRAY );
		}
		else if( that->parent->isResizeableY() )
		{
			bP.drawPixel( 7 , 0 , COLOR_GRAY );
			bP.drawHorizontalLine( 5 , 2 , 3 , COLOR_GRAY );
			bP.drawHorizontalLine( 3 , 4 , 5 , COLOR_GRAY );
			bP.drawHorizontalLine( 1 , 6 , 7 , COLOR_GRAY );
		}
		else if( that->parent->isResizeableX() )
		{
			bP.drawPixel( 0 , 7 , COLOR_GRAY );
			bP.drawVerticalLine( 2 , 5 , 3 , COLOR_GRAY );
			bP.drawVerticalLine( 4 , 3 , 5 , COLOR_GRAY );
			bP.drawVerticalLine( 6 , 1 , 7 , COLOR_GRAY );
		}
	}
	
	return use_default;
}

_callbackReturn _resizeHandle::updateHandler( _event event )
{
	// Receive Gadget
	_resizeHandle* that = event.getGadget<_resizeHandle>();
	
	if( !that->parent )
		return not_handled;
	
	if( that->parent->isResizeableX() || that->parent->isResizeableY() )
		that->show();
	else
		that->hide();
	
	return handled;
}

_callbackReturn _resizeHandle::dragHandler( _event event )
{
	// Receive Gadget
	_resizeHandle* that = event.getGadget<_resizeHandle>();
	
	that->parent->setSize( that->parent->getWidth() + event.getDeltaX() , that->parent->getHeight() + event.getDeltaY() );
	
	return handled;
}

_callbackReturn _resizeHandle::positionAdjuster( _event event )
{
	// Receive Gadget
	_resizeHandle* that = event.getGadget<_resizeHandle>();
	
	if( !that->parent )
		return not_handled;
	
	if( event == onParentRestyle )
		that->update();
	
	that->moveTo( that->parent->getWidth() - 9 , that->parent->getHeight() - 9 );
	
	return handled;
}

_resizeHandle::_resizeHandle( _style&& style ) :
	_gadget( _gadgetType::resizehandle , 8 , 8 , 0 , 0 , style | _styleAttr::draggable )
{
	this->setInternalEventHandler( onParentResize , make_callback(  &_resizeHandle::positionAdjuster ) );
	this->setInternalEventHandler( onParentSet , make_callback(  &_resizeHandle::positionAdjuster ) );
	this->setInternalEventHandler( onParentRestyle , make_callback(  &_resizeHandle::positionAdjuster ) );
	this->setInternalEventHandler( onDragging , make_callback( &_resizeHandle::dragHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( &_resizeHandle::refreshHandler ) );
	this->setInternalEventHandler( onUpdate , make_callback( &_resizeHandle::updateHandler ) );
}