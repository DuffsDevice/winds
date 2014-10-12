#include <_gadget/gadget.window.h>
#include <_gadget/gadget.window.menu.h>
#include <_gadget/gadget.window.menu.entry.h>
#include <_gadget/gadget.window.menu.entry.h>
#include <_type/type.font.glyphs.h>
#include <_controller/controller.gui.h>
#include <_controller/controller.localization.h>

_callbackReturn _windowMenu::refreshHandler( _event event )
{
	// Receive Gadget
	_windowMenu* that = event.getGadget<_windowMenu>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	bP.fill( _guiController::getControlBg() );
	bP.drawHorizontalLine( 0 , myH - 2 , myW , _color::white );
	bP.drawHorizontalLine( 0 , myH - 1 , myW , _guiController::getControlFg() );
	
	return use_default;
}

const _menu& _windowMenu::getStandardMenu(){
	static _menu ret;
	static bool initialized = false;
	
	if( !initialized )
	{
		ret.setList( { { 1 , { _localizationController::getBuiltInString("lbl_file") , 1 } } } );
		ret.setList( 1 ,
			{
				{ 1		, _localizationController::getBuiltInString("lbl_new") } ,
				{ 2		, _localizationController::getBuiltInString("lbl_open") } ,
				{ 3		, _localizationController::getBuiltInString("lbl_save") } ,
				{ 4		, _localizationController::getBuiltInString("lbl_save_as") } ,
				{ 100	, _menu::divider } ,
				{ 101	, _localizationController::getBuiltInString("lbl_exit") }
			}
		);
		initialized = true;
	}
	return ret;
}

void _windowMenu::generateChildren()
{
	this->removeChildren( true );
	
	for( const _pair<_u16,_menuEntry>& entry : this->getList() )
	{
		_windowMenuEntry* cM = new _windowMenuEntry( this , entry.second );
		
		cM->setInternalEventHandler( onParentAdd , _gadgetHelpers::moveBesidePrecedent( _direction::right , 0 , 0 , false , 0 , 0 ) );
		this->addChild( cM , true );
	}
}

_callbackReturn _windowMenu::updateHandler( _event event )
{
	// Receive Gadget
	_windowMenu* that = event.getGadget<_windowMenu>();
	
	_gadget* parent = that->getParent();
	
	if( !parent )
		return not_handled;
	
	_padding pad = that->isEnhanced() ? parent->getPadding() : _padding(0);
	
	that->setSizeIfAuto( parent->getWidth() - pad.left - pad.right , 0 );
	that->moveToIfAuto( pad.left , pad.top );
	
	return handled;
}

void _windowMenu::menuHandler( _u16 list , _u16 entry )
{
	if( list == 1 && entry == 101 ) // Close
	{
		_window* window = (_window*)this->getParent();
		if( window && ( window->getType() == _gadgetType::window || window->getType() == _gadgetType::mainframe ) )
			window->close();
	}
}

_windowMenu::_windowMenu( _menu menu , _style&& style ) :
	_menu( move(menu) )
	, _gadget( _gadgetType::windowmenu , ignore , ignore , ignore , _guiController::getListItemHeight() , move(style) )
{
	this->setMinHeight( _guiController::getListItemHeight() );
	
	// Add a handler to the standard Menu
	this->addHandler( _menuHandlerRule() , make_callback( this , &_windowMenu::menuHandler ) );
	
	// Register updatehandler
	this->setInternalEventHandler( onUpdate , make_callback( &_windowMenu::updateHandler ) );
	
	// Regsiter other handlers
	this->setInternalEventHandler( onParentResize , make_callback( &_windowMenu::updateHandler ) );
	this->setInternalEventHandler( onParentAdd , make_callback( &_windowMenu::updateHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( &_windowMenu::refreshHandler ) );
	
	// Generate list
	this->generateChildren();
}