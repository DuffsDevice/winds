#include "_gadget/gadget.contextmenu.entry.divider.h"
#include "_gadget/gadget.contextmenu.h"
#include "_type/type.system.h"

_contextMenuEntryDivider::_contextMenuEntryDivider( _style&& style ) :
	_contextMenuEntry( ignore , -1 , "----" , move(style) )
{
	this->setHeight( 3 );
	this->setX( 1 );
	
	// Register Handler
	this->setInternalEventHandler( onDraw , make_callback( &_contextMenuEntryDivider::refreshHandler ) );
	this->setInternalEventHandler( onParentResize , _gadgetHelpers::sizeParent( 2 , 0 ) );
	this->setInternalEventHandler( onParentAdd , _gadgetHelpers::eventForward(onParentResize) );
	this->removeInternalEventHandler( onUpdate );
	this->removeInternalEventHandler( onMouseDown );
	this->removeInternalEventHandler( onMouseClick );
	
	// Update & draw
	this->redraw();
}

_callbackReturn _contextMenuEntryDivider::refreshHandler( _event event )
{
	// Fetch Gadget
	_contextMenuEntryDivider* that = event.getGadget<_contextMenuEntryDivider>();
	
	_bitmapPort bP = that->getBitmapPort( event );
	
	// Fill Background
	bP.fill( _system::getRTA().getItemBackground() );
	
	_length myW = bP.getWidth();
	
	// Draw bevelled line
	bP.drawHorizontalLine( 0 , 1 , myW , _system::getRTA().getControlForeground() );
	
	return handled;
}