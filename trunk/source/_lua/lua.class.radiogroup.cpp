#include "_lua/lua.class.radiogroup.h"
#include "_lua/lua.gadget.radio.h"

/*##################################
##         Lua-RadioGroup        ##
##################################*/

// Lua-Ctor...
_lua_radiogroup::_lua_radiogroup( lua_State* L ) : _radiogroup()
{ }

//! addRadio
int _lua_radiogroup::addRadio(lua_State* L){ _lua_radio* r = (_lua_radio*)Lunar<_lua_radio>::check( L , 1 ); if( !r->gadget ) return 0; _radiogroup::addRadio( (_radio*)r->gadget , luaL_checkint( L , 2 ) ); return 0; }

//! removeRadio
int _lua_radiogroup::removeRadio(lua_State* L){ _lua_radio* r = (_lua_radio*)Lunar<_lua_radio>::check( L , 1 ); if( r->gadget ) _radiogroup::removeRadio( (_radio*)r->gadget ); return 0; }

//! enableRadio
int _lua_radiogroup::enableRadio(lua_State* L){ _lua_radio* r = (_lua_radio*)Lunar<_lua_radio>::check( L , 1 ); if( r->gadget ) _radiogroup::enableRadio( (_radio*)r->gadget ); return 0; }

//! setIntValue
int _lua_radiogroup::setIntValue(lua_State* L){ _radiogroup::setIntValue( luaL_checkint( L , 1 ) ); return 0; }

//! getIntValue
int _lua_radiogroup::getIntValue(lua_State* L){ lua_pushnumber( L , _radiogroup::getIntValue() ); return 1; }

//! Lua-_radiogroup
const char _lua_radiogroup::className[] = "_radiogroup";
Lunar<_lua_radiogroup>::FunctionType _lua_radiogroup::methods[] = {
	LUA_CLASS_FUNC(_lua_radiogroup, addRadio),
	LUA_CLASS_FUNC(_lua_radiogroup, removeRadio),
	LUA_CLASS_FUNC(_lua_radiogroup, enableRadio),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_radiogroup>::PropertyType _lua_radiogroup::properties[] = {
	{ "intValue" , &_lua_radiogroup::getIntValue , &_lua_radiogroup::setIntValue },
	LUA_CLASS_ATTR_END
};