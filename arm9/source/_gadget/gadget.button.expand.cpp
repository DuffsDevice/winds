#include "_gadget/gadget.button.expand.h"

// Graphics
#include "_resource/resource.icon.foldunfold.h"

_constBitmap _expandButton::graphic = BMP_FoldUnFoldIcon();

_callbackReturn _expandButton::refreshHandler( _event event )
{
	// Receive Gadget
	_expandButton* that = event.getGadget<_expandButton>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	// Content
	bP.copy( 0 , 0 , that->graphic );
	
	// Sign
	bP.drawHorizontalLine( 2 , 3 , 3 , COLOR_BLACK );
	
	if( !that->getIntValue() )
		bP.drawVerticalLine( 3 , 2 , 3 , COLOR_BLACK );
	
	
	return use_default;
}

_callbackReturn _expandButton::mouseHandler( _event event )
{
	// Receive Gadget
	_expandButton* that = event.getGadget<_expandButton>();
	
	// Change Value
	that->setIntValue( ! bool( that->getIntValue() ) );
	that->triggerEvent( onEdit );
	
	// Refresh
	that->redraw();
	
	return handled;
}

_expandButton::_expandButton( _optValue<_coord> x , _optValue<_coord> y , _style&& style ) :
	_gadget( _gadgetType::expandbutton , x , y , 7 , 7 , style | _styleAttr::notResizeable | _styleAttr::notDoubleClickable )
	, intValue( 0 )
{
	// Register my handlers
	this->setInternalEventHandler( onDraw , make_callback( &_expandButton::refreshHandler ) );
	this->setInternalEventHandler( onMouseClick , make_callback( &_expandButton::mouseHandler ) );
	this->setInternalEventHandler( onMouseEnter , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( onMouseLeave , _gadgetHelpers::eventForwardRefresh() );
	
	// Refresh Me
	this->redraw();
}