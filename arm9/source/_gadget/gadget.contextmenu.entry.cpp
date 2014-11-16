#include <_gadget/gadget.contextmenu.entry.h>
#include <_gadget/gadget.contextmenu.h>
#include <_controller/controller.font.h>
#include <_controller/controller.gui.h>
#include <_type/type.font.glyphs.h>

_contextMenuEntry::_contextMenuEntry( _optValue<_length> width , _u16 index , _menuEntry value , _style&& style ) :
	_gadget( _gadgetType::contextmenuentry , ignore , ignore , width , _guiController::getListItemHeight() , style | _style::notDoubleClickable )
	, entry( move(value) )
	, index( index )
{
	// Update The Size
	this->setInternalEventHandler( onUpdate , make_callback( &_contextMenuEntry::updateHandler ) );
	this->updateNow();
	
	// Register Handler
	this->setInternalEventHandler( onSelect , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( onDeselect , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( onDraw , make_callback( &_contextMenuEntry::refreshHandler ) );
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
	_fontHandle font = _fontController::getStandardFont();
	_u8 fontSize = _fontController::getStandardFontSize();
	
	// Fill Background
	bP.fill( _guiController::getItemBg( that->isSelected() ) );
	
	// Draw text
	bP.drawString( 2 , ( ( that->getHeight() - 1 ) >> 1 ) - ( ( font->getAscent( fontSize ) + 1 ) >> 1 ) , font , that->entry.text , _guiController::getItemFg( that->isSelected() ) );
	
	// If there is a sub menu connected
	if( that->entry.linkedList )
		// Draw little arrow
		bP.drawString( that->getWidth() - 5 , ( ( that->getHeight() - 1 ) >> 1 ) - ( ( font->getAscent( fontSize ) + 1 ) >> 1 ) , _fontController::getFont("SystemSymbols8") , string( 1 , _glyph::arrowRight ) , _guiController::getItemFg( that->isSelected() ) );
	
	return handled;
}

_callbackReturn _contextMenuEntry::updateHandler( _event event )
{
	_contextMenuEntry* that = event.getGadget<_contextMenuEntry>();
	
	_fontHandle font = _fontController::getStandardFont();
	_u8 fontSize = _fontController::getStandardFontSize();
	
	that->setWidthIfAuto( font->getStringWidth( that->entry.text , fontSize ) + ( that->entry.linkedList ? 9 : 3 ) );
	
	return handled;
}

_callbackReturn _contextMenuEntry::mouseHandler( _event event )
{
	_contextMenuEntry* that = event.getGadget<_contextMenuEntry>();
	
	// Fetch contextMenu instance
	_contextMenu* parent = (_contextMenu*) that->getParent();
	
	if( !parent || parent->getType() != _gadgetType::contextmenu )
		return not_handled;
	
	// Since this entry is already selected, submit the current value of the context menu
	parent->submit();
	
	return handled;
}