#include <_gadget/gadget.window.bar.h>
#include <_type/type.font.glyphs.h>
#include <_controller/controller.font.h>
#include <_controller/controller.gui.h>

_callbackReturn _windowBar::refreshHandler( _event event )
{
	// Receive Gadget
	_windowBar* that = event.getGadget<_windowBar>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myW = bP.getWidth();
	_length myH = bP.getHeight();
	
	bP.fill( that->bgColor );
	bP.drawHorizontalLine( 0 , 0 , myW , _guiController::getControlFg() );
	bP.drawHorizontalLine( 0 , 1 , myW , _color::white );
	
	_gadget* parent = that->getParent();
	
	if( parent )
	{
		_color col = _guiController::getControlFg();
		
		if( parent->isResizeableX() && parent->isResizeableY() )
			bP.drawChar( myW - 8 , myH - 8 , _fontController::getFont("SystemSymbols8") , _glyph::resizeHandleXY , col );
		else if( parent->isResizeableY() )
			bP.drawChar( myW - 7 , myH - 8 , _fontController::getFont("SystemSymbols8") , _glyph::resizeHandleY , col );
		else if( parent->isResizeableX() )
			bP.drawChar( myW - 8 , myH - 8 , _fontController::getFont("SystemSymbols8") , _glyph::resizeHandleX , col );
		else
			return use_default;
		
		bP.drawVerticalLine( myW - 8 , 2 , myH - 2 , _guiController::getControlFg() );
		bP.drawVerticalLine( myW - 7 , 2 , myH - 2 , _color::white );
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
	}
	
	_gadget* parent = that->getParent();
	
	if( parent )
		parent->setSize( parent->getWidth() + event.getDeltaX() , parent->getHeight() + event.getDeltaY() );
	
	return handled;
}

_callbackReturn _windowBar::updateHandler( _event event )
{
	// Receive Gadget
	_windowBar* that = event.getGadget<_windowBar>();
	_gadget* parent = that->getParent();
	
	if( !parent )
		return not_handled;
	
	_padding pad = parent->getPadding();
	
	that->moveToIfAuto( that->isEnhanced() ? pad.left : 0 , parent->getHeight() - that->getHeight() - pad.bottom - ( that->isEnhanced() ? 0 : pad.top ) );
	that->setSizeIfAuto(
		parent->getWidth() - pad.left - pad.right
		, 8
	);
	
	return handled;
}

_windowBar::_windowBar( _optValue<_length> height , _optValue<_color> bgColor , _style&& style ) :
	_gadget( _gadgetType::windowbar , ignore , ignore , ignore , height.isValid() ? (_length)height : 8 , style | _style::draggable | _style::noTransparentParts )
	, bgColor( bgColor.isValid() ? (_color)bgColor : _guiController::getControlBg() )
{
	this->setMinHeight( 8 );
	
	// Register updatehandler
	this->setInternalEventHandler( onUpdate , make_callback( &_windowBar::updateHandler ) );
	
	this->updateNow();
	
	// Regsiter other handlers
	this->setInternalEventHandler( onParentResize , make_callback( &_windowBar::updateHandler ) );
	this->setInternalEventHandler( onParentAdd , make_callback( &_windowBar::updateHandler ) );
	this->setInternalEventHandler( onParentRestyle , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( onResize , make_callback( &_windowBar::updateHandler ) );
	this->setInternalEventHandler( onDragStart , make_callback( &_windowBar::dragHandler ) );
	this->setInternalEventHandler( onDragging , make_callback( &_windowBar::dragHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( &_windowBar::refreshHandler ) );
}