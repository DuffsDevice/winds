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
int _lua_event::getType( lua_State* L ){ return push( L , eventType2string[_event::getType()] ); }

//! getGadget
int _lua_event::getGadget( lua_State* L ){ return push( L , _event::getGadget() ); }

//! Get Damaged Rects(The Rects that have to be repainted by the parent)
int _lua_event::getDamagedRects(lua_State* L){ return push( L , _event::getDamagedRects() ); }

//! Set Damaged Rects(The Rects that have to be repainted by the parent)
int _lua_event::setDamagedRects(lua_State* L){ _event::setDamagedRects( check<_area>( L , 1 ) ); return 0; }

//! Lua-_event
const char _lua_event::className[] = "Event";
Lunar<_lua_event>::FunctionType _lua_event::methods[] = {
	{ "resetParams" , wrap( _lua_event , &_event::resetParams ) },
	{ "hasClippingRects" , wrap( _lua_event , &_event::hasClippingRects ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_event>::PropertyType _lua_event::properties[] = {
	{ "type"		, &_lua_event::getType							, &_lua_event::setType },
	{ "gadget" 		, &_lua_event::getGadget						, wrap( _lua_event , &_event::setGadget ) },
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