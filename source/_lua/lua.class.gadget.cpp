#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.class.event.h"
#include "_lua/lua.class.rect.h"
#include "_lua/lua.class.bitmapPort.h"
#include "_lua/lua.class.border.h"
#include "_lua/lua.gadget.window.h"
#include "_lua/lua.gadget.button.h"
#include "_lua/lua.gadget.calendar.h"
#include "_lua/lua.gadget.label.h"
#include "_lua/lua.gadget.progressbar.h"
#include "_lua/lua.gadget.counter.h"
#include "_lua/lua.gadget.colorpicker.h"
#include "_lua/lua.gadget.checkbox.h"
#include "_lua/lua.gadget.imagegadget.h"
#include "_lua/lua.gadget.scrollArea.h"
#include "_lua/lua.gadget.scrollBar.h"
#include "_lua/lua.gadget.stickybutton.h"
#include "_lua/lua.gadget.textbox.h"
#include "_lua/lua.gadget.select.h"
#include "_lua/lua.gadget.radio.h"
#include "_type/type.system.h"

/*##################################
##           Lua-Gadget           ##
##################################*/

_lua_gadget* _lua_gadget::getLuaGadget( lua_State* L , int narg ){
	
	_lua_gadget* tmp;

	if( ( tmp = Lunar< _lua_window >::lightcheck( L , narg ) ) != nullptr )
		return tmp;
	if( ( tmp = Lunar< _lua_button >::lightcheck( L , narg ) ) != nullptr )
		return tmp;
	if( ( tmp = Lunar< _lua_stickybutton >::lightcheck( L , narg ) ) != nullptr )
		return tmp;
	if( ( tmp = Lunar< _lua_label >::lightcheck( L , narg ) ) != nullptr )
		return tmp;
	if( ( tmp = Lunar< _lua_checkbox >::lightcheck( L , narg ) ) != nullptr )
		return tmp;
	if( ( tmp = Lunar< _lua_select >::lightcheck( L , narg ) ) != nullptr )
		return tmp;
	if( ( tmp = Lunar< _lua_textbox >::lightcheck( L , narg ) ) != nullptr )
		return tmp;
	if( ( tmp = Lunar< _lua_counter >::lightcheck( L , narg ) ) != nullptr )
		return tmp;
	if( ( tmp = Lunar< _lua_progressbar >::lightcheck( L , narg ) ) != nullptr )
		return tmp;
	if( ( tmp = Lunar< _lua_radio >::lightcheck( L , narg ) ) != nullptr )
		return tmp;
	if( ( tmp = Lunar< _lua_calendar >::lightcheck( L , narg ) ) != nullptr )
		return tmp;
	if( ( tmp = Lunar< _lua_imagegadget >::lightcheck( L , narg ) ) != nullptr )
		return tmp;
	if( ( tmp = Lunar< _lua_scrollarea >::lightcheck( L , narg ) ) != nullptr )
		return tmp;
	if( ( tmp = Lunar< _lua_scrollbar >::lightcheck( L , narg ) ) != nullptr )
		return tmp;
	if( ( tmp = Lunar< _lua_colorpicker >::lightcheck( L , narg ) ) != nullptr )
		return tmp;
	
	return Lunar<_lua_gadget>::check( L , narg );
}

_lua_gadget::_lua_gadget( _gadget* g , bool wasAllocated ) :
	gadget( g ),
	wasAllocated( wasAllocated )
{}

void _lua_gadget::setGadget( _gadget* g , bool wasAllocated ){
	if( this->wasAllocated && this->gadget )
		delete this->gadget;
	
	// Register new gadget
	this->gadget = g;
	this->wasAllocated = wasAllocated;
}

//! Lua-Ctor
_lua_gadget::_lua_gadget( lua_State* L ) :
	gadget( new _gadget( _gadgetType::_plain , luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) ) ),
	wasAllocated( true )
{ }

//! Lua-Dtor
_lua_gadget::~_lua_gadget()
{
	//printf("%sDeleting %s\n",this->wasAllocated?"":"Not ",gadgetType2string[ this->gadget->getType() ].c_str());
	//_system::submit();
	
	if( this->gadget != nullptr && this->wasAllocated )
		delete this->gadget;
}

//! bubbleRefresh
int _lua_gadget::bubbleRefresh(lua_State* L){ this->gadget->bubbleRefresh( luaL_optboolean( L , 1 , false ) ); return 0; }

//! refreshBitmap
int _lua_gadget::refreshBitmap( lua_State* L ){ this->gadget->refreshBitmap(); return 0; }

//! getBitmap
int _lua_gadget::getBitmap( lua_State* L ){ Lunar<_lua_bitmap>::push( L , new _lua_bitmap( &this->gadget->getBitmap() ) ); return 1; }

//! getBitmapPort
int _lua_gadget::getBitmapPort( lua_State* L ){ Lunar<_lua_bitmapPort>::push( L , new _lua_bitmapPort( this->gadget->getBitmapPort() ) ); return 1; }

//! getScreen
int _lua_gadget::getScreen( lua_State* L ){ _gadget* g = this->gadget->getScreen(); if( !g ) return 0; Lunar<_lua_gadget>::push( L , new _lua_gadget( g , false ) ); return 1; }

//! getToppestChild
int _lua_gadget::getToppestChild( lua_State* L ){ _gadget* g = this->gadget->getToppestChild(); if( !g ) return 0; Lunar<_lua_gadget>::push( L , new _lua_gadget( g , false ) ); return 1; }

//! setUserEventHandler
int _lua_gadget::setUserEventHandler( lua_State* L ){
	this->gadget->setUserEventHandler( string2eventType[ luaL_checkstring( L , 1 ) ] , _luaCallback( L , 2 ) );
	return 0;
}

//! setInternalEventHandler
int _lua_gadget::setInternalEventHandler( lua_State* L ){ 
	this->gadget->setInternalEventHandler( string2eventType[ luaL_checkstring( L , 1 ) ] , _luaCallback( L , 2 ) );
	return 0;
}

//! removeUserHandler
int _lua_gadget::removeUserEventHandler( lua_State* L ){
	_eventType t = string2eventType[ luaL_checkstring( L , 1 ) ];
	this->gadget->removeUserEventHandler( t );
	return 0;
}

//! removeInternalHandler
int _lua_gadget::removeInternalEventHandler( lua_State* L ){
	_eventType t = string2eventType[ luaL_checkstring( L , 1 ) ];
	this->gadget->removeInternalEventHandler( t );
	return 0;
}

//! triggerEvent
int _lua_gadget::triggerEvent(lua_State* L){  _lua_event* e = Lunar<_lua_event>::check( L , 1 ); if( e ) this->gadget->triggerEvent( *e ); return 0; }

//! canReactTo
int _lua_gadget::canReactTo( lua_State* L ){ lua_pushboolean( L , this->gadget->canReactTo( string2eventType[ luaL_checkstring( L , 1 ) ] ) ); return 1; }

//! handleEvent
int _lua_gadget::handleEvent( lua_State* L ){ _lua_event* e = Lunar<_lua_event>::check( L , 1 ); if( !e ) return 0; lua_pushstring( L , callbackReturn2string[ this->gadget->handleEvent( *e , luaL_optboolean( L , 2 , false ) ) ].c_str() ); return 1; }

//! handleEventUser
int _lua_gadget::handleEventUser( lua_State* L ){ _lua_event* e = Lunar<_lua_event>::check( L , 1 ); if( !e ) return 0; lua_pushstring( L , callbackReturn2string[ this->gadget->handleEventUser( *e ) ].c_str() ); return 1; }

//! handleEventInternal
int _lua_gadget::handleEventInternal( lua_State* L ){ _lua_event* e = Lunar<_lua_event>::check( L , 1 ); if( !e ) return 0; lua_pushstring( L , callbackReturn2string[ this->gadget->handleEventInternal( *e ) ].c_str() ); return 1; }

//! handleEventDefault
int _lua_gadget::handleEventDefault( lua_State* L ){ _lua_event* e = Lunar<_lua_event>::check( L , 1 ); if( !e ) return 0; lua_pushstring( L , callbackReturn2string[ this->gadget->handleEventDefault( *e ) ].c_str() ); return 1; }

//! Get absolute X
int _lua_gadget::getAbsoluteX(lua_State* L){ lua_pushnumber( L , this->gadget->getAbsoluteX() ); return 1; }

//! Get absolute Y
int _lua_gadget::getAbsoluteY(lua_State* L){ lua_pushnumber( L , this->gadget->getAbsoluteY() ); return 1; }

//! Get absolute Position
int _lua_gadget::getAbsolutePosition(lua_State* L){ _2s32 pos = this->gadget->getAbsolutePosition(); lua_pushnumber( L , pos.first ); lua_pushnumber( L , pos.second ); return 2; }

//! Get X
int _lua_gadget::getX(lua_State* L){ lua_pushnumber( L , this->gadget->getX() ); return 1; }

//! Get Y
int _lua_gadget::getY(lua_State* L){ lua_pushnumber( L , this->gadget->getY() ); return 1; }

//! Set X
int _lua_gadget::setX(lua_State* L){ this->gadget->setX( luaL_checkint( L , 1 ) ); return 0; }

//! Set Y
int _lua_gadget::setY(lua_State* L){ this->gadget->setY( luaL_checkint( L , 1 ) ); return 0; }

//! moveTo
int _lua_gadget::moveTo(lua_State* L){ this->gadget->moveTo( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) ); return 0; }

//! moveRelative
int _lua_gadget::moveRelative(lua_State* L){ this->gadget->moveRelative( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) ); return 0; }

//! getParent
int _lua_gadget::getParent( lua_State* L ){ if( !this->gadget ) return 0; Lunar<_lua_gadget>::push( L , new _lua_gadget( this->gadget->getParent() , false ) ); return 1; }

//! setParent
int _lua_gadget::setParent( lua_State* L ){ _lua_gadget* g = NULL; if( ( g = this->getLuaGadget( L , 1 ) ) != NULL ) this->gadget->setParent( g->gadget ); return 0; }

//! enhanceToParent
int _lua_gadget::enhanceToParent( lua_State* L ){ _lua_gadget* g = NULL; if( ( g = this->getLuaGadget( L , 1 ) ) != NULL ) this->gadget->enhanceToParent( g->gadget ); return 0; }

//! removeChild
int _lua_gadget::removeChild( lua_State* L ){ _lua_gadget* g = NULL; if( ( g = this->getLuaGadget( L , 1 ) ) != NULL ) this->gadget->removeChild( g->gadget ); return 0; }

//! removeChildren
int _lua_gadget::removeChildren( lua_State* L ){ this->gadget->removeChildren(); return 0; }

//! removeEnhancedChildren
int _lua_gadget::removeEnhancedChildren( lua_State* L ){ this->gadget->removeEnhancedChildren(); return 0; }

//! addChild
int _lua_gadget::addChild( lua_State* L ){ _lua_gadget* g = this->getLuaGadget( L , 1 ); if( g ) this->gadget->addChild( g->gadget ); return 0; }

//! addEnhancedChild
int _lua_gadget::addEnhancedChild( lua_State* L ){ _lua_gadget* g = NULL; if( ( g = this->getLuaGadget( L , 1 ) ) != NULL ) this->gadget->addEnhancedChild( g->gadget ); return 0; }

//! getDimensions
int _lua_gadget::getDimensions( lua_State* L ){ Lunar<_lua_rect>::push( L , new _lua_rect( this->gadget->getDimensions() ) ); return 1; }

//! getAbsoluteDimensions
int _lua_gadget::getAbsoluteDimensions( lua_State* L ){ Lunar<_lua_rect>::push( L , new _lua_rect( this->gadget->getAbsoluteDimensions() ) ); return 1; }

//! setDimensions
int _lua_gadget::setDimensions( lua_State* L ){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( rc ) this->gadget->setDimensions( *rc ); return 0; }

//! getHeight
int _lua_gadget::getHeight( lua_State* L ){ lua_pushnumber( L , this->gadget->getHeight() ); return 1; }

//! getWidth
int _lua_gadget::getWidth( lua_State* L ){ lua_pushnumber( L , this->gadget->getWidth() ); return 1; }

//! setWidth
int _lua_gadget::setWidth(lua_State* L){ this->gadget->setWidth( luaL_checkint( L , 1 ) ); return 0; }

//! setHeight
int _lua_gadget::setHeight(lua_State* L){ this->gadget->setHeight( luaL_checkint( L , 1 ) ); return 0; }

//! toDerived
int _lua_gadget::toDerived( lua_State* L ){ 
	switch( this->gadget->getType() ){
		case _gadgetType::window:
			Lunar<_lua_window>::push( L , new _lua_window( (_window*)this->gadget ) );
			return 1;
		case _gadgetType::checkbox:
			Lunar<_lua_checkbox>::push( L , new _lua_checkbox( (_checkbox*)this->gadget ) );
			return 1;
		case _gadgetType::button:
			Lunar<_lua_button>::push( L , new _lua_button( (_button*)this->gadget ) );
			return 1;
		default:
			break;
	}
	return 0;
}

//! setPadding
int _lua_gadget::setPadding(lua_State* L){ _lua_border* rc = Lunar<_lua_border>::check( L , 1 ); if( rc ) this->gadget->setPadding( *(_border*)rc ); return 0; }

//! getPadding
int _lua_gadget::getPadding(lua_State* L){ Lunar<_lua_border>::push( L , new _lua_border( this->gadget->getPadding() ) ); return 1; }

//! applyStyle
int _lua_gadget::applyStyle(lua_State* L){ applyString2style( this->gadget->style , luaL_checkstring( L , 1 ) ); this->gadget->triggerEvent( onStyleSet ); return 0; }

//! hide
int _lua_gadget::hide(lua_State* L){ this->gadget->hide(); return 0; }

//! show
int _lua_gadget::show(lua_State* L){ this->gadget->show(); return 0; }

//! focus
int _lua_gadget::focus(lua_State* L){ this->gadget->focus(); return 0; }

//! blur
int _lua_gadget::blur(lua_State* L){ this->gadget->blur(); return 0; }

//! isEnhanced
int _lua_gadget::isEnhanced(lua_State* L){ lua_pushboolean( L , this->gadget->isEnhanced() ); return 1; }

//! isInvisible
int _lua_gadget::isInvisible(lua_State* L){ lua_pushboolean( L , this->gadget->isInvisible() ); return 1; }

//! isHidden
int _lua_gadget::isHidden(lua_State* L){ lua_pushboolean( L , this->gadget->isHidden() ); return 1; }

//! isMinimized
int _lua_gadget::isMinimized(lua_State* L){ lua_pushboolean( L , this->gadget->isMinimized() ); return 1; }

//! isResizeable
int _lua_gadget::isResizeable(lua_State* L){ lua_pushboolean( L , this->gadget->isResizeable() ); return 1; }

//! isResizeableX
int _lua_gadget::isResizeableX(lua_State* L){ lua_pushboolean( L , this->gadget->isResizeableX() ); return 1; }

//! isResizeableY
int _lua_gadget::isResizeableY(lua_State* L){ lua_pushboolean( L , this->gadget->isResizeableY() ); return 1; }

//! isMinimizeable
int _lua_gadget::isMinimizeable(lua_State* L){ lua_pushboolean( L , this->gadget->isMinimizeable() ); return 1; }

//! isMaximized
int _lua_gadget::isMaximized(lua_State* L){ lua_pushboolean( L , this->gadget->isMaximized() ); return 1; }

//! hasSmallDragTrig
int _lua_gadget::hasSmallDragTrig(lua_State* L){ lua_pushboolean( L , this->gadget->hasSmallDragTrig() ); return 1; }

//! isMouseClickRepeat
int _lua_gadget::isMouseClickRepeat(lua_State* L){ lua_pushboolean( L , this->gadget->isMouseClickRepeat() ); return 1; }

//! isDoubleClickable
int _lua_gadget::isDoubleClickable(lua_State* L){ lua_pushboolean( L , this->gadget->isDoubleClickable() ); return 1; }

//! requestsKeyboard
int _lua_gadget::requestsKeyboard(lua_State* L){ lua_pushboolean( L , this->gadget->requestsKeyboard() ); return 1; }

//! hasFocus
int _lua_gadget::hasFocus(lua_State* L){ lua_pushboolean( L , this->gadget->hasFocus() ); return 1; }

//! getSize
int _lua_gadget::getSize(lua_State* L){ Lunar<_lua_rect>::push( L , new _lua_rect( this->gadget->getSize() ) ); return 1; }

//! getType
int _lua_gadget::getType(lua_State* L){ lua_pushstring( L , gadgetType2string[this->gadget->getType()].c_str() ); return 1; }

//! Lua-_gadget
const char _lua_gadget::className[] = "_gadget";
Lunar<_lua_gadget>::FunctionType _lua_gadget::methods[] = {
	GADGET_FUNCS( _lua_gadget ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_gadget>::PropertyType _lua_gadget::properties[] = {
	GADGET_ATTRS( _lua_gadget ),
	LUA_CLASS_ATTR_END
};