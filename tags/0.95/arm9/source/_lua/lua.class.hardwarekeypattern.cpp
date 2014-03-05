#include "_lua/lua.class.hardwarekeypattern.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##           Lua-Border           ##
##################################*/

_lua_hardwarekeypattern::_lua_hardwarekeypattern( const _hardwareKeyPattern& hwkp ) :
	_hardwareKeyPattern( hwkp )
{}

//! Constructor
_lua_hardwarekeypattern::_lua_hardwarekeypattern( lua_State* L ) : 
	_hardwareKeyPattern( check<int>( L , 1 ) )
{}

//! isHardwareKey
int _lua_hardwarekeypattern::isHardwareKey(lua_State* L){
	return push(
		L ,
		_hardwareKeyPattern::isHardwareKey( check<_key>( L , 1 ) )
	);
}


//! Lua-_rect
const char _lua_hardwarekeypattern::className[] = "HardwareKeyPattern";
Lunar<_lua_hardwarekeypattern>::FunctionType _lua_hardwarekeypattern::methods[] = {
	{ "isKeySet" , wrap( _lua_hardwarekeypattern , &_hardwareKeyPattern::isKeySet ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_hardwarekeypattern>::PropertyType _lua_hardwarekeypattern::properties[] = {
	LUA_CLASS_ATTR_END
};

Lunar<_lua_hardwarekeypattern>::StaticType		staticmethods[] = {
	{ "isHardwareKey" , &_lua_hardwarekeypattern::isHardwareKey }
};