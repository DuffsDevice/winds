#include "_lua/lua.class.event.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.area.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##         Lua-GadgetEvent        ##
##################################*/
// Ctor...
_lua_event::_lua_event( _event e ) :
	_event( e )
{ }

// Lua-Ctor...
_lua_event::_lua_event( lua_State* L ) : 
	_event( string2eventType[ lightcheck<string>( L , 1 , "_none_" ) ] )
{ }

//! setType
int _lua_event::setType( lua_State* L ){ _event::setType( string2eventType[ check<string>( L , 1 ) ] ); return 0; }

//! getType
int _lua_event::getType( lua_State* L ){ lua_pushstring( L , eventType2string[ _event::getType() ].c_str() ); return 1; }

//! getGadget
int _lua_event::getGadget( lua_State* L ){ if( !_event::getGadget() ) return 0; Lunar<_lua_gadget>::push( L , new _lua_gadget( _event::getGadget() , false ) ); return 1; }

//! resetParams
int _lua_event::resetParams(lua_State* L){ _event::resetParams(); return 0; }

//! getDestination
int _lua_event::getDestination(lua_State* L){ if( !_event::getDestination() ) return 0; Lunar<_lua_gadget>::push( L , new _lua_gadget( _event::getDestination() , false ) ); return 1; }

//! setDestination
int _lua_event::setDestination(lua_State* L){ _lua_gadget* g = Lunar<_lua_gadget>::check( L , 1 ); if( g ) _event::setDestination( g->getGadget() ); return 0; }

//! Set Pressure
int _lua_event::setPressure(lua_State* L){ _event::setPressure( check<int>( L , 1 ) ); return 0; }

//! Get Pressure
int _lua_event::getPressure(lua_State* L){ return push( L , _event::getPressure() ); }

//! Set X
int _lua_event::setPosX(lua_State* L){ _event::setPosX( check<int>( L , 1 ) ); return 0; }

//! Set Y
int _lua_event::setPosY(lua_State* L){ _event::setPosY( check<int>( L , 1 ) ); return 0; }

//! Get X
int _lua_event::getPosX(lua_State* L){ lua_pushnumber( L , _event::getPosX() ); return 1; }

//! Get Y
int _lua_event::getPosY(lua_State* L){ lua_pushnumber( L , _event::getPosY() ); return 1; }

//! Set X
int _lua_event::setDeltaX(lua_State* L){ _event::setDeltaX( check<int>( L , 1 ) ); return 0; }

//! Set Y
int _lua_event::setDeltaY(lua_State* L){ _event::setDeltaY( check<int>( L , 1 ) ); return 0; }

//! Get X
int _lua_event::getDeltaX(lua_State* L){ lua_pushnumber( L , _event::getDeltaX() ); return 1; }

//! Get Y
int _lua_event::getDeltaY(lua_State* L){ lua_pushnumber( L , _event::getDeltaY() ); return 1; }

//! Get Key-Code
int _lua_event::getKeyCode(lua_State* L){ lua_pushnumber( L , _event::getKeyCode() ); return 1; }

//! Set Key-Code
int _lua_event::setKeyCode(lua_State* L){ _event::setKeyCode( check<int>( L , 1 ) ); return 0; }

//! Get Held Time of the key that triggered the Event
int _lua_event::getHeldTime(lua_State* L){ lua_pushnumber( L , _event::getHeldTime() ); return 1; }

//! Set Held Time of the key that triggered the Event
int _lua_event::setHeldTime(lua_State* L){ _event::setHeldTime( check<int>( L , 1 ) ); return 0; }

//! Get KeyCode State of that Moment the Event was triggered
int _lua_event::getCurrentKeys(lua_State* L){ lua_pushnumber( L , _event::getCurrentKeys() ); return 1; }

//! Set KeyCode State of that Moment the Event was triggered
int _lua_event::setCurrentKeys(lua_State* L){ _event::setCurrentKeys( check<int>( L , 1 ) ); return 0; }

//! Set Damaged Rects(The Rects that have to be repainted by the parent)
int _lua_event::setDamagedRects(lua_State* L){ _lua_area* a = Lunar<_lua_area>::check( L , 1 ); if( a ) _event::setDamagedRects( *a ); return 0; }

//! Get Damaged Rects(The Rects that have to be repainted by the parent)
int _lua_event::getDamagedRects(lua_State* L){  Lunar<_lua_area>::push( L , new _lua_area( _event::getDamagedRects() ) ); return 1; }

//! Check if event has Clipping Rects
int _lua_event::hasClippingRects(lua_State* L){ lua_pushboolean( L , _event::hasClippingRects() ); return 1; }

//! Lua-_event
const char _lua_event::className[] = "Event";
Lunar<_lua_event>::FunctionType _lua_event::methods[] = {
	LUA_CLASS_FUNC(_lua_event, getGadget),
	LUA_CLASS_FUNC(_lua_event, resetParams),
	LUA_CLASS_FUNC(_lua_event, hasClippingRects),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_event>::PropertyType _lua_event::properties[] = {
	{ "type" , &_lua_event::getType , &_lua_event::setType },
	{ "destination"	, wrap( _lua_event , &_event::getDestination )	, wrap( _lua_event , &_event::setDestination ) },
	{ "pressure"	, wrap( _lua_event , &_event::getPressure )		, wrap( _lua_event , &_event::setPressure ) },
	{ "posX"		, wrap( _lua_event , &_event::getPosX )			, wrap( _lua_event , &_event::setPosX ) },
	{ "posY"		, wrap( _lua_event , &_event::getPosY )			, wrap( _lua_event , &_event::setPosY ) },
	{ "deltaX"		, wrap( _lua_event , &_event::getDeltaX )		, wrap( _lua_event , &_event::setDeltaX ) },
	{ "deltaY"		, wrap( _lua_event , &_event::getDeltaY )		, wrap( _lua_event , &_event::setDeltaY ) },
	{ "keyCode"		, wrap( _lua_event , &_event::getKeyCode )		, wrap( _lua_event , &_event::setKeyCode ) },
	{ "heldTime"	, wrap( _lua_event , &_event::getHeldTime )		, wrap( _lua_event , &_event::setHeldTime ) },
	{ "currentKeys"	, wrap( _lua_event , &_event::getCurrentKeys )	, wrap( _lua_event , &_event::setCurrentKeys ) },
	{ "damagedRects", &_lua_event::getDamagedRects					, &_lua_event::setDamagedRects },
	LUA_CLASS_ATTR_END
};