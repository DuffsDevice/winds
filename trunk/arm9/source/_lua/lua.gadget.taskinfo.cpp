#include "_lua/lua.gadget.taskinfo.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##          Lua-TaskInfo          ##
##################################*/

_lua_taskinfo::_lua_taskinfo( lua_State* L ) :
	_lua_gadget( new _taskInfo( check<_bitmap>( L , 1 ) , lightcheck<_style>( L , 2 ) ) )
{}

int _lua_taskinfo::displayNotification( lua_State* L )
{
	_taskInfo* tI = this->getGadget<_taskInfo>();
	
	if( is_a<_voidCallback>( L , 3 ) )
		tI->displayNotification( check<string>( L , 1 ) , check<string>( L , 2 ) , check<_voidCallback>( L , 3 ) , lightcheck<_bitmap>( L , 4 ) );
	else
		tI->displayNotification( check<string>( L , 1 ) , check<string>( L , 2 ) , lightcheck<_bitmap>( L , 3 ) );
	
	return 0;
}

//! Lua-taskInfo
const char _lua_taskinfo::className[] = "TaskInfo";
Lunar<_lua_taskinfo>::FunctionType _lua_taskinfo::methods[] = {
	{ "displayNotification" , &_lua_taskinfo::displayNotification },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_taskinfo>::PropertyType _lua_taskinfo::properties[] = {
	{ "image" , wrap( _lua_taskinfo , &_taskInfo::getImage ) , wrap( _lua_taskinfo , &_taskInfo::setImage ) },
	LUA_CLASS_ATTR_END
};