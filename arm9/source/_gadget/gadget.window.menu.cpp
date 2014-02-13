#include "_gadget/gadget.window.h"
#include "_gadget/gadget.window.menu.h"
#include "_gadget/gadget.window.menu.entry.h"
#include "_gadget/gadget.window.menu.entry.h"
#include "_type/type.system.h"
#include "_type/type.font.glyphs.h"

_callbackReturn _windowMenu::refreshHandler( _event event )
{
	// Receive Gadget
	_windowMenu* that = event.getGadget<_windowMenu>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	bP.fill( _system::getRTA().getControlBackground() );
	bP.drawHorizontalLine( 0 , myH - 2 , myW , _color::white );
	bP.drawHorizontalLine( 0 , myH - 1 , myW , _system::getRTA().getControlForeground() );
	
	return use_default;
}

const _menu& _windowMenu::getStandardMenu(){
	static _menu ret;
	static bool initialized = false;
	
	if( !initialized )
	{
		ret.setMainList( { { 1 , _system::getLocalizedString("lbl_file") } } );
		ret.setList( 1 ,
			{
				{ 0 , _system::getLocalizedString("lbl_open") } ,
				{ 1 , _system::getLocalizedString("lbl_save") } ,
				{ 2 , _system::getLocalizedString("lbl_save_as") } ,
				{ 3 , "----" } ,
				{ 4 , _system::getLocalizedString("lbl_exit") }
			}
		);
		initialized = true;
	}
	return ret;
}

void _windowMenu::generateChildren()
{
	this->removeChildren( true );
	
	for( const _pair<_int,string>& entry : this->getMainList() )
	{
		_windowMenuEntry* cM = new _windowMenuEntry( entry.first , *this );
		
		cM->setInternalEventHandler( onParentSet , _gadgetHelpers::moveBesidePrecedent( _direction::right , 0 , 0 , false , 0 , 0 ) );
		this->addChild( cM , true );
	}
}

_callbackReturn _windowMenu::updateHandler( _event event )
{
	// Receive Gadget
	_windowMenu* that = event.getGadget<_windowMenu>();
	
	if( !that->parent )
		return not_handled;
	
	_padding pad = that->isEnhanced() ? that->parent->getPadding() : _padding(0);
	
	that->setSizeIfAuto( that->parent->getWidth() - pad.left - pad.right , 0 );
	that->moveToIfAuto( pad.left , pad.top );
	
	return handled;
}

void _windowMenu::menuHandler( _int list , _int entry )
{
	if( list == 1 && entry == 4 ) // Close
	{
		_window* window = (_window*)this->getParent();
		if( window && window->getType() == _gadgetType::window )
			window->close();
	}
}

_windowMenu::_windowMenu( const _menu& menu , _style&& style ) :
	_menu( menu ) , _gadget( _gadgetType::windowmenu , ignore , ignore , ignore , _system::getUser().lIH , move(style) )
{
	this->setMinHeight( _system::getUser().lIH );
	
	// Add a handler to the standard Menu
	this->addMenuHandler( _menuHandlerRule() , make_callback( this , &_windowMenu::menuHandler ) );
	
	// Register updatehandler
	this->setInternalEventHandler( onUpdate , make_callback( &_windowMenu::updateHandler ) );
	
	this->updateNow();
	
	// Regsiter other handlers
	this->setInternalEventHandler( onParentResize , make_callback( &_windowMenu::updateHandler ) );
	this->setInternalEventHandler( onParentSet , make_callback( &_windowMenu::updateHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( &_windowMenu::refreshHandler ) );
	
	// Generate list
	this->generateChildren();
}