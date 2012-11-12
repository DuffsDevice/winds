#include "_lua/lua.class.event.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.area.h"

/*##################################
##         Lua-GadgetEvent        ##
##################################*/
// Ctor...
_lua_event::_lua_event( _event e ) :
	_event( e )
{ }

// Lua-Ctor...
_lua_event::_lua_event( lua_State* L ) : _event( ){
	// _event( type , args )
	if( lua_isstring( L , 1 ) )
		_event::setType( string2eventType[ luaL_checkstring( L , 1 ) ] );
	// _event( src* , type , args )
	else if( lua_isuserdata( L , 1 ) )
	{
		_lua_gadget* g = Lunar<_lua_gadget>::check( L , 1 );
		
		if( g )
			_event::setSource( g->gadget );
		
		// Is a gadgetEventType?
		if( lua_isstring( L , 2 ) )
			_event::setType( string2eventType[ luaL_checkstring( L , 2 ) ] );
	}
}

//! setType
int _lua_event::setType( lua_State* L ){ _event::setType( string2eventType[ luaL_checkstring( L , 1 ) ] ); return 0; }

//! getType
int _lua_event::getType( lua_State* L ){ lua_pushstring( L , eventType2string[ _event::getType() ].c_str() ); return 1; }

//! getGadget
int _lua_event::getGadget( lua_State* L ){ Lunar<_lua_gadget>::push( L , new _lua_gadget( _event::getGadget() ) ); return 1; }

//! resetParams
int _lua_event::resetParams(lua_State* L){ _event::resetParams(); return 0; }

//! getDestination
int _lua_event::getDestination(lua_State* L){  Lunar<_lua_gadget>::push( L , new _lua_gadget( (_gadget*)_event::getDestination() ) ); return 1; }

//! getSource
int _lua_event::getSource(lua_State* L){  Lunar<_lua_gadget>::push( L , new _lua_gadget( (_gadget*)_event::getSource() ) ); return 1; }

//! setDestination
int _lua_event::setDestination(lua_State* L){ _lua_gadget* g = Lunar<_lua_gadget>::check( L , 1 ); if( g ) _event::setDestination( g->gadget ); return 0; }

//! setSource
int _lua_event::setSource(lua_State* L){  _lua_gadget* g = Lunar<_lua_gadget>::check( L , 1 ); if( g ) _event::setSource( g->gadget ); return 0; }

//! Set X
int _lua_event::setPosX(lua_State* L){ _event::setPosX( luaL_checkint( L , 1 ) ); return 0; }

//! Set Y
int _lua_event::setPosY(lua_State* L){ _event::setPosY( luaL_checkint( L , 1 ) ); return 0; }

//! Get X
int _lua_event::getPosX(lua_State* L){ lua_pushnumber( L , _event::getPosX() ); return 1; }

//! Get Y
int _lua_event::getPosY(lua_State* L){ lua_pushnumber( L , _event::getPosY() ); return 1; }

//! Set X
int _lua_event::setDeltaX(lua_State* L){ _event::setDeltaX( luaL_checkint( L , 1 ) ); return 0; }

//! Set Y
int _lua_event::setDeltaY(lua_State* L){ _event::setDeltaY( luaL_checkint( L , 1 ) ); return 0; }

//! Get X
int _lua_event::getDeltaX(lua_State* L){ lua_pushnumber( L , _event::getDeltaX() ); return 1; }

//! Get Y
int _lua_event::getDeltaY(lua_State* L){ lua_pushnumber( L , _event::getDeltaY() ); return 1; }

//! Get Key-Code
int _lua_event::getKeyCode(lua_State* L){ lua_pushnumber( L , _event::getKeyCode() ); return 1; }

//! Set Key-Code
int _lua_event::setKeyCode(lua_State* L){ _event::setKeyCode( luaL_checkint( L , 1 ) ); return 0; }

//! Get Held Time of the key that triggered the Event
int _lua_event::getHeldTime(lua_State* L){ lua_pushnumber( L , _event::getHeldTime() ); return 1; }

//! Set Held Time of the key that triggered the Event
int _lua_event::setHeldTime(lua_State* L){ _event::setHeldTime( luaL_checkint( L , 1 ) ); return 0; }

//! Get KeyCode State of that Moment the Event was triggered
int _lua_event::getCurrentKeyCodes(lua_State* L){ lua_pushnumber( L , _event::getCurrentKeyCodes() ); return 1; }

//! Set KeyCode State of that Moment the Event was triggered
int _lua_event::setCurrentKeyCodes(lua_State* L){ _event::setCurrentKeyCodes( luaL_checkint( L , 1 ) ); return 0; }

//! Set Damaged Rects(The Rects that have to be repainted by the parent)
int _lua_event::setDamagedRects(lua_State* L){ _lua_area* a = Lunar<_lua_area>::check( L , 1 ); if( a ) _event::setDamagedRects( *a ); return 0; }

//! Get Damaged Rects(The Rects that have to be repainted by the parent)
int _lua_event::getDamagedRects(lua_State* L){  Lunar<_lua_area>::push( L , new _lua_area( _event::getDamagedRects() ) ); return 1; }

//! Check if event is a bubble-Refresh-One
int _lua_event::isBubblePrevented(lua_State* L){ lua_pushboolean( L , _event::isBubblePrevented() ); return 1; }

//! Check if event is a bubble-Refresh-One
int _lua_event::preventBubble(lua_State* L){ _event::preventBubble( luaL_checkint( L , 1 ) ); return 0; }

//! Check if event has Clipping Rects
int _lua_event::hasClippingRects(lua_State* L){ lua_pushboolean( L , _event::hasClippingRects() ); return 1; }

//! Lua-_event
const char _lua_event::className[] = "_event";
Lunar<_lua_event>::FunctionType _lua_event::methods[] = {
	LUA_CLASS_FUNC(_lua_event, getGadget),
	LUA_CLASS_FUNC(_lua_event, resetParams),
	LUA_CLASS_FUNC(_lua_event, isBubblePrevented),
	LUA_CLASS_FUNC(_lua_event, preventBubble),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_event>::PropertyType _lua_event::properties[] = {
	LUA_CLASS_ATTR( _lua_event , Type , "type" ),
	LUA_CLASS_ATTR( _lua_event , Source , "source" ),
	LUA_CLASS_ATTR( _lua_event , Destination , "destination" ),
	LUA_CLASS_ATTR( _lua_event , PosX , "posX" ),
	LUA_CLASS_ATTR( _lua_event , PosY , "posY" ),
	LUA_CLASS_ATTR( _lua_event , DeltaX , "deltaX" ),
	LUA_CLASS_ATTR( _lua_event , DeltaY , "deltaY" ),
	LUA_CLASS_ATTR( _lua_event , KeyCode , "keyCode" ),
	LUA_CLASS_ATTR( _lua_event , HeldTime , "heldTime" ),
	LUA_CLASS_ATTR( _lua_event , CurrentKeyCodes , "currentKeyCodes" ),
	LUA_CLASS_ATTR( _lua_event , DamagedRects , "damagedRects" ),
	LUA_CLASS_ATTR_END
};