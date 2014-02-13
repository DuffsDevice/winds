#include "_gadget/gadget.resizehandle.h"
#include "_type/type.system.h"
#include "_type/type.font.glyphs.h"

_callbackReturn _resizeHandle::refreshHandler( _event event )
{
	// Receive Gadget
	_resizeHandle* that = event.getGadget<_resizeHandle>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( that->bgColor );
	
	if( that->parent )
	{
		_color col = _system::getRTA().getControlForeground();
		
		if( that->parent->isResizeableX() && that->parent->isResizeableY() )
			bP.drawChar( 0 , 0 , _system::getFont("SystemSymbols8") , _glyph::resizeHandleXY , col );
		else if( that->parent->isResizeableY() )
			bP.drawChar( 1 , 0 , _system::getFont("SystemSymbols8") , _glyph::resizeHandleY , col );
		else if( that->parent->isResizeableX() )
			bP.drawChar( 0 , 0 , _system::getFont("SystemSymbols8") , _glyph::resizeHandleX , col );
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
	
	_padding pad = that->isEnhanced() ? that->parent->getPadding() : _padding(0);
	
	that->moveTo( that->parent->getWidth() - 8 - pad.right , that->parent->getHeight() - 8 - pad.bottom );
	
	return handled;
}

_resizeHandle::_resizeHandle( _optValue<_color> bgColor , _style&& style ) :
	_gadget( _gadgetType::resizehandle , 8 , 8 , 0 , 0 , style | _styleAttr::draggable )
	, bgColor( bgColor.isValid() ? (_color)bgColor : _color::transparent )
{
	this->setInternalEventHandler( onParentResize , make_callback(  &_resizeHandle::positionAdjuster ) );
	this->setInternalEventHandler( onParentSet , make_callback(  &_resizeHandle::positionAdjuster ) );
	this->setInternalEventHandler( onParentRestyle , make_callback(  &_resizeHandle::positionAdjuster ) );
	this->setInternalEventHandler( onDragging , make_callback( &_resizeHandle::dragHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( &_resizeHandle::refreshHandler ) );
	this->setInternalEventHandler( onUpdate , make_callback( &_resizeHandle::updateHandler ) );
}