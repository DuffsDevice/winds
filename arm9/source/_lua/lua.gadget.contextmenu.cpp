#include "_lua/lua.gadget.contextmenu.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##           Lua-Select           ##
##################################*/

_lua_contextmenu::_lua_contextmenu( lua_State* L ) : 
	_lua_gadget( new _contextMenu( optcheck<int>( L , 1 ) , check<_menuEntryList>( L , 2 ) , lightcheck<_gadget*>( L , 3 , nullptr ) , lightcheck<bool>( L , 4 , false ) , lightcheck<int>( L , 5 , -1 ) , lightcheck<_style>( L , 6 , _style() ) ) )
{}

//! addIndex
int _lua_contextmenu::addIndex( lua_State* L ){
	_contextMenu* cM = (_contextMenu*)(_gadget*)(_lua_gadget*)this;
	cM->getList()[ check<int>( L , 1 ) ] = check<string>( L , 2 );
	cM->update();
	return 0;
}

//! removeIndex
int _lua_contextmenu::removeIndex( lua_State* L ){
	_contextMenu* cM = (_contextMenu*)(_gadget*)(_lua_gadget*)this;
	cM->getList().erase( check<int>( L , 1 ) );
	cM->update();
	return 0;
}

//! clearList
int _lua_contextmenu::clearList( lua_State* L ){
	_contextMenu* cM = (_contextMenu*)(_gadget*)(_lua_gadget*)this;
	cM->getList().clear();
	cM->update();
	return 0;
}

//! getEntryFromNumber
int _lua_contextmenu::getEntryFromNumber( lua_State* L ){
	_contextMenu* cM = (_contextMenu*)(_gadget*)(_lua_gadget*)this;
	lua_pushstring( L , cM->getList()[ check<int>(L,1) ].c_str() );
	return 1;
}

//! Lua-button
const char _lua_contextmenu::className[] = "Select";
Lunar<_lua_contextmenu>::FunctionType _lua_contextmenu::methods[] = {
	LUA_CLASS_FUNC(_lua_contextmenu,addIndex),
	LUA_CLASS_FUNC(_lua_contextmenu,removeIndex),
	LUA_CLASS_FUNC(_lua_contextmenu,clearList),
	LUA_CLASS_FUNC(_lua_contextmenu,getEntryFromNumber),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_contextmenu>::PropertyType _lua_contextmenu::properties[] = {
	GADGET_BASE_ATTR,
	{ "strValue" , wrap( _lua_contextmenu , &_contextMenu::getStrValue ) , nullptr },
	{ "value" , wrap( _lua_contextmenu , &_contextMenu::getIntValue ) , wrap( _lua_contextmenu , &_contextMenu::setIntValue ) },
	{ "list" , wrap( _lua_contextmenu , &_contextMenu::getList ) , wrap( _lua_contextmenu , &_contextMenu::setList ) },
	LUA_CLASS_ATTR_END
};