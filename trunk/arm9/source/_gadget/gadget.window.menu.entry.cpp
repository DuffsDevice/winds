#include <_gadget/gadget.window.menu.entry.h>
#include <_gadget/gadget.window.menu.h>
#include <_gadget/gadget.contextmenu.h>
#include <_controller/controller.font.h>
#include <_controller/controller.gui.h>

_windowMenuEntry::_windowMenuEntry( _menu* menu , _menuEntry menuEntry , _style&& style ) :
	_gadget( _gadgetType::windowmenuentry , ignore , ignore , ignore , _guiController::getListItemHeight() - 2 , style | _style::noTransparentParts )
	, entryIndex( move(menuEntry) )
	, menuList( entryIndex.linkedList ? new _contextMenu( ignore , menu , this , false , -1 , entryIndex.linkedList ) : nullptr )
{
	// Update The Size
	this->setInternalEventHandler( onUpdate , make_callback( &_windowMenuEntry::updateHandler ) );
	this->updateNow();
	
	// Register Handler
	this->setInternalEventHandler( onDraw , make_callback( &_windowMenuEntry::refreshHandler ) );
	this->setInternalEventHandler( onMouseDown , make_callback( &_windowMenuEntry::mouseHandler ) );
	
	if( this->menuList )
		this->menuList->setUserEventHandler( onClose , _gadgetHelpers::eventForwardRefreshGadget( this ) );
	
	// Update & draw
	this->redraw();
}

_callbackReturn _windowMenuEntry::refreshHandler( _event event )
{
	// Fetch Gadget
	_windowMenuEntry* that = event.getGadget<_windowMenuEntry>();
	
	_bitmapPort bP = that->getBitmapPort( event );
	
	// Fetch Font data
	_fontHandle font = _fontController::getStandardFont();
	_u8 fontSize = _fontController::getStandardFontSize();
	
	bool opened = that->menuList ? that->menuList->isOpened() : false;
	
	// Fill Background
	bP.fill( opened ? _guiController::getControlFg() : _guiController::getControlBg() );
	
	// Draw text
	bP.drawString( 2 , ( ( that->getHeight() - 1 ) >> 1 ) - ( ( font->getAscent( fontSize ) + 1 ) >> 1 ) , font , that->entryIndex.text , _color::black );
	
	return handled;
}

_callbackReturn _windowMenuEntry::updateHandler( _event event )
{
	_windowMenuEntry* that = event.getGadget<_windowMenuEntry>();
	
	_fontHandle font = _fontController::getStandardFont();
	_u8 fontSize = _fontController::getStandardFontSize();
	
	// Adjust width
	that->setWidthIfAuto( font->getStringWidth( that->entryIndex.text , fontSize ) + 3 );
	
	return handled;
}

_callbackReturn _windowMenuEntry::mouseHandler( _event event )
{
	_windowMenuEntry* that = event.getGadget<_windowMenuEntry>();
	
	if( that->menuList )
	{
		// Compute position of contextmenu
		_pos pos = that->getAbsolutePosition();
		
		// Toggle the cM
		that->menuList->toggle( pos.first , pos.second + that->getHeight() + 1 );
		
		// Refresh appearence
		that->redraw();
	}
	
	return handled;
}