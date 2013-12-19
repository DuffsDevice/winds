#include "_gadget/gadget.window.bar.h"
#include "_type/type.system.h"
#include "_type/type.font.glyphs.h"

_callbackReturn _windowBar::refreshHandler( _event event )
{
	// Receive Gadget
	_windowBar* that = event.getGadget<_windowBar>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myW = bP.getWidth();
	_length myH = bP.getHeight();
	
	bP.fill( that->bgColor );
	bP.drawHorizontalLine( 0 , 0 , myW , _system::getRTA().getControlForeground() );
	bP.drawHorizontalLine( 0 , 1 , myW , COLOR_WHITE );
	
	if( that->parent )
	{
		_pixel col = _system::getRTA().getControlForeground();
		
		if( that->parent->isResizeableX() && that->parent->isResizeableY() )
			bP.drawChar( myW - 8 , myH - 8 , _system::getFont("SystemSymbols8") , _glyph::resizeHandleXY , col );
		else if( that->parent->isResizeableY() )
			bP.drawChar( myW - 7 , myH - 8 , _system::getFont("SystemSymbols8") , _glyph::resizeHandleY , col );
		else if( that->parent->isResizeableX() )
			bP.drawChar( myW - 8 , myH - 8 , _system::getFont("SystemSymbols8") , _glyph::resizeHandleX , col );
		else
			return use_default;
		
		bP.drawVerticalLine( myW - 8 , 2 , myH - 2 , _system::getRTA().getControlForeground() );
		bP.drawVerticalLine( myW - 7 , 2 , myH - 2 , COLOR_WHITE );
	}
	
	return use_default;
}

_callbackReturn _windowBar::dragHandler( _event event )
{
	// Receive Gadget
	_windowBar* that = event.getGadget<_windowBar>();
	
	if( event == onDragStart )
	{
		if( event.getPosX() < that->getWidth() - 8 )
			return prevent_default;
		//return handled;
	}
	
	that->parent->setSize( that->parent->getWidth() + event.getDeltaX() , that->parent->getHeight() + event.getDeltaY() );
	
	return handled;
}

_callbackReturn _windowBar::updateHandler( _event event )
{
	// Receive Gadget
	_windowBar* that = event.getGadget<_windowBar>();
	
	if( !that->parent )
		return not_handled;
	
	_padding pad = that->parent->getPadding();
	
	that->moveToIfAuto( that->isEnhanced() ? pad.left : 0 , that->parent->getHeight() - that->getHeight() - pad.bottom - ( that->isEnhanced() ? 0 : pad.top ) );
	that->setSizeIfAuto(
		that->parent->getWidth() - pad.left - pad.right
		, 8
	);
	
	return handled;
}

_windowBar::_windowBar( _optValue<_length> height , _optValue<_pixel> bgColor , _style&& style ) :
	_gadget( _gadgetType::windowbar , ignore , ignore , ignore , height.isValid() ? (_length)height : 8 , style | _styleAttr::draggable )
	, bgColor( bgColor.isValid() ? (_pixel)bgColor : _system::getRTA().getControlBackground() )
{
	this->setMinHeight( 8 );
	
	// Register updatehandler
	this->setInternalEventHandler( onUpdate , make_callback( &_windowBar::updateHandler ) );
	
	this->updateNow();
	
	// Regsiter other handlers
	this->setInternalEventHandler( onParentResize , make_callback( &_windowBar::updateHandler ) );
	this->setInternalEventHandler( onParentSet , make_callback( &_windowBar::updateHandler ) );
	this->setInternalEventHandler( onParentRestyle , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( onResize , make_callback( &_windowBar::updateHandler ) );
	this->setInternalEventHandler( onDragStart , make_callback( &_windowBar::dragHandler ) );
	this->setInternalEventHandler( onDragging , make_callback( &_windowBar::dragHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( &_windowBar::refreshHandler ) );
}