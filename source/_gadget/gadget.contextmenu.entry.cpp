#include "_gadget/gadget.contextmenu.h"
#include "_type/type.system.h"

_contextMenuEntry::_contextMenuEntry( _s32 id , string value , _optValue<_length>&& width , _style&& style ) :
	_gadget( _gadgetType::contextmenuentry , move(width) , _system::getUser()->fOH , ignore , ignore , move(style) )
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
	this->setInternalEventHandler( onParentSet , _gadgetHelpers::moveBesidePrecedent() );
	
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
	_u8 fontSize =_system::_rtA_->getDefaultFontSize();
	
	_contextMenu* parent = ((_contextMenu*)that->parent);
	
	bool drawHighlighted = parent ? that == parent->activeEntry : false;
	
	// Fill Background
	bP.fill( drawHighlighted ? RGB255( 10 , 36 , 106 ) : COLOR_WHITE );
	
	// Draw text
	bP.drawString( 2 , ( ( that->getHeight() - 1 ) >> 1 ) - ( ( font->getAscent( fontSize ) + 1 ) >> 1 ) , font , that->text , drawHighlighted ? COLOR_WHITE : COLOR_BLACK );
	
	return handled;
}

_callbackReturn _contextMenuEntry::updateHandler( _event event )
{
	_contextMenuEntry* that = event.getGadget<_contextMenuEntry>();
	
	if( that->hasAutoWidth() )
	{
		const _font* font = _system::getFont();
		_u8 fontSize =_system::_rtA_->getDefaultFontSize();
		
		that->setWidthInternal( font->getStringWidth( that->text , fontSize ) + 3 );
	}
	
	return handled;
}

_callbackReturn _contextMenuEntry::mouseHandler( _event event )
{
	_contextMenuEntry* that = event.getGadget<_contextMenuEntry>();
	
	// Fetch contextMenu instance
	_contextMenu* parent = (_contextMenu*)that->parent;
	
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