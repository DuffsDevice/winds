#include "_gadget/gadget.window.menu.entry.h"
#include "_gadget/gadget.window.menu.h"
#include "_gadget/gadget.contextmenu.h"
#include "_type/type.system.h"

_windowMenuEntry::_windowMenuEntry( _int listIndex , _menu& menu  , _style&& style ) :
	_gadget( _gadgetType::windowmenuentry , ignore , ignore , ignore , _system::getUser().lIH - 2 , move(style) )
	, text( menu.getMainList()[listIndex] )
	, listReference( listIndex )
	, menuList( new _contextMenu( ignore , menu.getList( listIndex ) , this ) )
{
	// Update The Size
	this->setInternalEventHandler( onUpdate , make_callback( &_windowMenuEntry::updateHandler ) );
	this->updateNow();
	
	// Register Handler
	this->setInternalEventHandler( onDraw , make_callback( &_windowMenuEntry::refreshHandler ) );
	this->setInternalEventHandler( onMouseDown , make_callback( &_windowMenuEntry::mouseHandler ) );
	this->menuList->setInternalEventHandler( onClose , _gadgetHelpers::eventForwardRefreshGadget( this ) );
	
	// A click to an entry in the contextmenu will forward the
	// event to this _windowMenuEntry which will then call the handler
	this->menuList->setInternalEventHandler( onEdit , _gadgetHelpers::eventForwardGadget( this ) );
	this->setInternalEventHandler( onEdit , make_callback( &_windowMenuEntry::selectHandler ) );
	
	// Update & draw
	this->redraw();
}

_callbackReturn _windowMenuEntry::selectHandler( _event event )
{
	_windowMenuEntry* that = event.getGadget<_windowMenuEntry>();
	_windowMenu* menu = (_windowMenu*)that->getParent();
	
	if( menu->getType() == _gadgetType::windowmenu )
		menu->callHandler( that->listReference , that->menuList->getIntValue() );
	
	return handled;
}

_callbackReturn _windowMenuEntry::refreshHandler( _event event )
{
	// Fetch Gadget
	_windowMenuEntry* that = event.getGadget<_windowMenuEntry>();
	
	_bitmapPort bP = that->getBitmapPort( event );
	
	// Fetch Font data
	const _font* font = _system::getFont();
	_u8 fontSize = _system::getRTA().getDefaultFontSize();
	
	// Fill Background
	bP.fill( that->menuList->isOpened() ? _system::getRTA().getControlForeground() : _system::getRTA().getControlBackground() );
	
	// Draw text
	bP.drawString( 2 , ( ( that->getHeight() - 1 ) >> 1 ) - ( ( font->getAscent( fontSize ) + 1 ) >> 1 ) , font , that->text , _color::black );
	
	return handled;
}

_callbackReturn _windowMenuEntry::updateHandler( _event event )
{
	_windowMenuEntry* that = event.getGadget<_windowMenuEntry>();
	
	const _font* font = _system::getFont();
	_u8 fontSize = _system::getRTA().getDefaultFontSize();
	
	that->setWidthIfAuto( font->getStringWidth( that->text , fontSize ) + 3 );
	
	return handled;
}

_callbackReturn _windowMenuEntry::mouseHandler( _event event )
{
	_windowMenuEntry* that = event.getGadget<_windowMenuEntry>();
	
	// Compute position of contextmenu
	_2s32 pos = that->getAbsolutePosition();
	
	// Toggle the cM
	that->menuList->toggle( pos.first , pos.second + that->getHeight() + 1 );
	
	// Refresh appearence
	that->redraw();
	
	return handled;
}