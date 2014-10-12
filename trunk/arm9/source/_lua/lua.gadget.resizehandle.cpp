#include <_lua/lua.gadget.resizehandle.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*##################################
##        Lua-ResizeHandle        ##
##################################*/

_lua_resizehandle::_lua_resizehandle( lua_State* L ) : 
	_lua_gadget( new _resizeHandle( optcheck<_color>( L , 1 ) , lightcheck<_style>( L , 2 ) ) )
{ }

//! Lua-window
const char _lua_resizehandle::className[] = "ResizeHandle";
Lunar<_lua_resizehandle>::FunctionType _lua_resizehandle::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_resizehandle>::PropertyType _lua_resizehandle::properties[] = {
	{ "bgColor" , wrap( _lua_resizehandle , &_resizeHandle::getBgColor ) , wrap( _lua_resizehandle , &_resizeHandle::setBgColor ) },
	LUA_CLASS_ATTR_END
};