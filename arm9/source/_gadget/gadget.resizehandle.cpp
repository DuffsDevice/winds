#include <_gadget/gadget.resizehandle.h>
#include <_controller/controller.font.h>
#include <_controller/controller.gui.h>

_callbackReturn _resizeHandle::refreshHandler( _event event )
{
	// Receive Gadget
	_resizeHandle* that = event.getGadget<_resizeHandle>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( that->bgColor );
	
	_gadget* parent = that->getParent();
	
	if( parent )
	{
		_color col = _guiController::getControlFg();
		_fontHandle ft = _fontController::getFont("System7");
		
		if( parent->isResizeableX() && parent->isResizeableY() )
			bP.drawChar( 0 , 0 , ft , L'\u25A8' , col );
		else if( parent->isResizeableY() )
			bP.drawChar( 1 , 0 , ft , L'\u25A4' , col );
		else if( parent->isResizeableX() )
			bP.drawChar( 0 , 0 , ft , L'\u25A5' , col );
	}
	
	return use_default;
}

_callbackReturn _resizeHandle::updateHandler( _event event )
{
	// Receive Gadget
	_resizeHandle* that = event.getGadget<_resizeHandle>();
	
	_gadget* parent = that->getParent();
	
	if( !parent )
		return not_handled;
	
	if( parent->isResizeableX() || parent->isResizeableY() )
		that->show();
	else
		that->hide();
	
	return handled;
}

_callbackReturn _resizeHandle::dragHandler( _event event )
{
	// Receive Gadget
	_resizeHandle* that = event.getGadget<_resizeHandle>();
	
	_gadget* parent = that->getParent();
	
	if( !parent )
		return not_handled;
	
	parent->setSize( parent->getWidth() + event.getDeltaX() , parent->getHeight() + event.getDeltaY() );
	
	return handled;
}

_callbackReturn _resizeHandle::positionAdjuster( _event event )
{
	// Receive Gadget
	_resizeHandle* that = event.getGadget<_resizeHandle>();
	
	_gadget* parent = that->getParent();
	
	if( !parent )
		return not_handled;
	
	if( event == onParentRestyle )
		that->update();
	
	_padding pad = that->isEnhanced() ? parent->getPadding() : _padding(0);
	
	that->moveTo( parent->getWidth() - 8 - pad.right , parent->getHeight() - 8 - pad.bottom );
	
	return handled;
}

_resizeHandle::_resizeHandle( _optValue<_color> bgColor , _style&& style ) :
	_gadget( _gadgetType::resizehandle , 8 , 8 , 0 , 0 , style | _style::draggable )
	, bgColor( bgColor.isValid() ? (_color)bgColor : _color::transparent )
{
	this->setInternalEventHandler( onParentResize , make_callback(  &_resizeHandle::positionAdjuster ) );
	this->setInternalEventHandler( onParentAdd , make_callback(  &_resizeHandle::positionAdjuster ) );
	this->setInternalEventHandler( onParentRestyle , make_callback(  &_resizeHandle::positionAdjuster ) );
	this->setInternalEventHandler( onDragging , make_callback( &_resizeHandle::dragHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( &_resizeHandle::refreshHandler ) );
	this->setInternalEventHandler( onUpdate , make_callback( &_resizeHandle::updateHandler ) );
}