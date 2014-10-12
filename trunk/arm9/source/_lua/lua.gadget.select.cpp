#include <_lua/lua.gadget.select.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*##################################
##           Lua-Select           ##
##################################*/

_lua_select::_lua_select( lua_State* L ) :
	_lua_gadget( new _select( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , optcheck<int>( L , 3 ) , lightcheck<_menuEntryList>( L , 4 ) , lightcheck<int>( L , 5 , -1 ) , lightcheck<_style>( L , 6 ) ) )
{}

//! addIndex
int _lua_select::addIndex( lua_State* L ){
	_select* slc = this->getGadget<_select>();
	slc->getList()[ check<int>( L , 1 ) ] = check<string>( L , 2 );
	slc->update();
	return 0;
}

//! removeIndex
int _lua_select::removeIndex( lua_State* L ){
	_select* slc = this->getGadget<_select>();
	slc->getList().erase( check<int>( L , 1 ) );
	slc->update();
	return 0;
}

//! clearList
int _lua_select::clearList( lua_State* L ){
	_select* slc = this->getGadget<_select>();
	slc->getList().clear();
	slc->update();
	return 0;
}

//! getEntryFromNumber
int _lua_select::getEntryFromNumber( lua_State* L ){
	_select* slc = this->getGadget<_select>();
	return push( L , slc->getList()[check<int>(L,1)] );
}

//! Lua-button
const char _lua_select::className[] = "Select";
Lunar<_lua_select>::FunctionType _lua_select::methods[] = {
	LUA_CLASS_FUNC(_lua_select,addIndex),
	LUA_CLASS_FUNC(_lua_select,removeIndex),
	LUA_CLASS_FUNC(_lua_select,clearList),
	LUA_CLASS_FUNC(_lua_select,getEntryFromNumber),
	LUA_CLASS_FUNC_END
};

using helperType1 = _menuEntryList& (_select::*)();

Lunar<_lua_select>::PropertyType _lua_select::properties[] = {
	{ "strValue"	, wrap( _lua_select , &_select::getStrValue )	, nullptr },
	{ "value"		, wrap( _lua_select , &_select::getIntValue )	, wrap( _lua_select , &_select::setIntValue ) },
	{ "list"		, wrap( _lua_select , (::helperType1) &_select::getList )		, wrap( _lua_select , &_select::setList ) },
	LUA_CLASS_ATTR_END
};