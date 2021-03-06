#include <_lua/lua.class.radiogroup.h>
#include <_lua/lua.gadget.radio.h>
#include <_lua/lua.func.h>
using namespace _luafunc;

/*##################################
##         Lua-RadioGroup        ##
##################################*/

// Lua-Ctor...
_lua_radiogroup::_lua_radiogroup( lua_State* L ) : _singleValueGroup<_radio>()
{ }

//! addRadio
int _lua_radiogroup::addRadio(lua_State* L){ _lua_radio* r = (_lua_radio*)Lunar<_lua_radio>::check( L , 1 ); if( !r->getGadget() ) return 0; _singleValueGroup<_radio>::addSelector( (_radio*)r->getGadget() , check<int>( L , 2 ) ); return 0; }

//! removeRadio
int _lua_radiogroup::removeRadio(lua_State* L){ _lua_radio* r = (_lua_radio*)Lunar<_lua_radio>::check( L , 1 ); if( r->getGadget() ) _singleValueGroup<_radio>::removeSelector( (_radio*)r->getGadget() ); return 0; }

//! enableRadio
int _lua_radiogroup::enableRadio(lua_State* L){ _lua_radio* r = (_lua_radio*)Lunar<_lua_radio>::check( L , 1 ); if( r->getGadget() ) _singleValueGroup<_radio>::enableSelector( (_radio*)r->getGadget() ); return 0; }

//! setIntValue
int _lua_radiogroup::setValue(lua_State* L){ _singleValueGroup<_radio>::setIntValue( check<int>( L , 1 ) ); return 0; }

//! getIntValue
int _lua_radiogroup::getValue(lua_State* L){ push( L , _singleValueGroup<_radio>::getIntValue() ); return 1; }

//! Lua-_radiogroup
const char _lua_radiogroup::className[] = "RadioGroup";
Lunar<_lua_radiogroup>::FunctionType _lua_radiogroup::methods[] = {
	LUA_CLASS_FUNC(_lua_radiogroup, addRadio),
	LUA_CLASS_FUNC(_lua_radiogroup, removeRadio),
	LUA_CLASS_FUNC(_lua_radiogroup, enableRadio),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_radiogroup>::PropertyType _lua_radiogroup::properties[] = {
	{ "value" , &_lua_radiogroup::getValue , &_lua_radiogroup::setValue },
	LUA_CLASS_ATTR_END
};