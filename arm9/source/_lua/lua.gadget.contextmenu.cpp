#include <_lua/lua.gadget.contextmenu.h>
#include <_lua/lua.class.menu.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

#include <_controller/controller.debug.h>

/*##################################
##         Lua-ContextMenu        ##
##################################*/

_lua_contextmenu::_lua_contextmenu( lua_State* L ) : 
	_lua_gadget( new _contextMenu( optcheck<_length>( L , 1 ) , check<_menu>( L , 2 ) , lightcheck<_gadget*>( L , 3 , nullptr ) , lightcheck<bool>( L , 4 , false ) , lightcheck<int>( L , 5 , -1 ) , optcheck<int>( L , 6 ) , lightcheck<_style>( L , 7 ) ) )
{}

//! Lua-button
const char _lua_contextmenu::className[] = "ContextMenu";
Lunar<_lua_contextmenu>::FunctionType _lua_contextmenu::methods[] = {
	LUA_CLASS_FUNC_END
};

using helperType1 = _menu& (_contextMenu::*)();
using helperType2 = void (_contextMenu::*)( _paramAlloc<_menu> );
using helperType3 = _menuEntryList& (_contextMenu::*)();

Lunar<_lua_contextmenu>::PropertyType _lua_contextmenu::properties[] = {
	{ "strValue"	, wrap( _lua_contextmenu , &_contextMenu::getStrValue )				, nullptr },
	{ "value"		, wrap( _lua_contextmenu , &_contextMenu::getIntValue )				, wrap( _lua_contextmenu , &_contextMenu::setIntValue ) },
	{ "currentList"	, wrap( _lua_contextmenu , (::helperType3) &_contextMenu::getList ) , nullptr },
	{ "listIndex"	, wrap( _lua_contextmenu , &_contextMenu::getListIndex ) 			, wrap( _lua_contextmenu , &_contextMenu::setListIndex ) },
	{ "menu"		, wrap( _lua_contextmenu , (::helperType1) &_contextMenu::getMenu )	, wrap( _lua_contextmenu , (::helperType2) &_contextMenu::setMenu ) },
	LUA_CLASS_ATTR_END
};