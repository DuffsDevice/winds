#include "_gadget/gadget.contextmenu.entry.h"
#include "_gadget/gadget.contextmenu.h"
#include "_type/type.system.h"

_contextMenuEntry::_contextMenuEntry( _optValue<_length> width , _int id , string value , _style&& style ) :
	_gadget( _gadgetType::contextmenuentry , ignore , ignore , width , _system::getUser()->lIH , move(style) )
	, text( move(value) )
	, id( id )
{
	// Update The Size
	this->setInternalEventHandler( onUpdate , make_callback( &_contextMenuEntry::updateHandler ) );
	this->updateNow();
	
	// Register Handler
	this->setInternalEventHandler( onDraw , make_callback( &_contextMenuEntry::refreshHandler ) );
	this->setInternalEventHandler( onMouseDown, make_callback( &_contextMenuEntry::mouseHandler ) );
	this->setInternalEventHandler( onMouseClick , make_callback( &_contextMenuEntry::mouseHandler ) );
	
	// Update & draw
	this->redraw();
}

_callbackReturn _contextMenuEntry::refreshHandler( _event event )
{
	// Fetch Gadget
	_contextMenuEntry* that = event.getGadget<_contextMenuEntry>();
	
	_bitmapPort bP = that->getBitmapPort( event );
	
	// Fetch Font data
	const _font* font = _system::getFont();
	_u8 fontSize = _system::getRTA().getDefaultFontSize();
	
	_contextMenu* parent = ((_contextMenu*)that->parent);
	
	bool drawHighlighted = parent && parent->getType() == _gadgetType::contextmenu ? that == parent->activeEntry : false;
	
	// Fill Background
	bP.fill( _system::getRTA().getItemBackground( drawHighlighted ) );
	
	// Draw text
	bP.drawString( 2 , ( ( that->getHeight() - 1 ) >> 1 ) - ( ( font->getAscent( fontSize ) + 1 ) >> 1 ) , font , that->text , _system::getRTA().getItemForeground( drawHighlighted ) );
	
	return handled;
}

_callbackReturn _contextMenuEntry::updateHandler( _event event )
{
	_contextMenuEntry* that = event.getGadget<_contextMenuEntry>();
	
	const _font* font = _system::getFont();
	_u8 fontSize = _system::getRTA().getDefaultFontSize();
	
	that->setWidthIfAuto( font->getStringWidth( that->text , fontSize ) + 3 );
	
	return handled;
}

_callbackReturn _contextMenuEntry::mouseHandler( _event event )
{
	_contextMenuEntry* that = event.getGadget<_contextMenuEntry>();
	
	// Fetch contextMenu instance
	_contextMenu* parent = (_contextMenu*)that->parent;
	
	if( parent->getType() != _gadgetType::contextmenu )
		return not_handled;
	
	if( parent )
	{
		// Only close the contextMenu if the event is an 'onMouseClick'
		if( event == onMouseClick )
			parent->selectEntry( that );
		else
			parent->highlightEntry( that );
	}
	
	return handled;
}