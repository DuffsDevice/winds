#include "_lua/lua.gadget.resizehandle.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##        Lua-ResizeHandle        ##
##################################*/

_lua_resizehandle::_lua_resizehandle( lua_State* L ) : 
	_lua_gadget( new _resizeHandle( lightcheck<_style>( L , 1 , _style() ) ) )
{ }

//! Lua-window
const char _lua_resizehandle::className[] = "ResizeHandle";
Lunar<_lua_resizehandle>::FunctionType _lua_resizehandle::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_resizehandle>::PropertyType _lua_resizehandle::properties[] = {
	GADGET_BASE_ATTR,
	LUA_CLASS_ATTR_END
};