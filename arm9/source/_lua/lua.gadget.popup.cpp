#include "_lua/lua.gadget.popup.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##            Lua-Popup           ##
##################################*/

_lua_popup::_lua_popup( lua_State* L ) :
	_lua_gadget( new _popup( optcheck<_coord>( L , 1 ) , optcheck<_coord>( L , 2 ) , lightcheck<_gadget*>( L , 3 ) , lightcheck<_style>( L , 4 ) ) )
{}

//! show
int _lua_popup::show( lua_State* L ){
	if( is_a<_rect>( L , 1 ) )
		this->getGadget<_popup>()->show( check<_rect>( L , 1 ) , lightcheck<bool>( L , 2 , true ) );
	else
		this->getGadget<_popup>()->show( check<_coord>( L , 1 ) , check<_coord>( L , 2 ) , lightcheck<bool>( L , 3 , true ) );
	return 0;
}

//! showCentered
int _lua_popup::showCentered( lua_State* L ){
	this->getGadget<_popup>()->showCentered( check<_pos>( L , 1 ) );
	return 0;
}

//! shelve
int _lua_popup::shelve( lua_State* L ){
	this->getGadget<_popup>()->shelve( lightcheck<bool>( L , 1 , false ) );
	return 0;
}

//! toggle
int _lua_popup::toggle( lua_State* L ){
	if( is_a<_rect>( L , 1 ) )
		this->getGadget<_popup>()->toggle( check<_rect>( L , 1 ) , lightcheck<bool>( L , 2 , true ) );
	else
		this->getGadget<_popup>()->toggle( check<_coord>( L , 1 ) , check<_coord>( L , 2 ) , lightcheck<bool>( L , 3 , true ) );
	return 0;
}

//! Lua-button
const char _lua_popup::className[] = "Popup";
Lunar<_lua_popup>::FunctionType _lua_popup::methods[] = {
	{ "show"			, &_lua_popup::show },
	{ "showCentered"	, &_lua_popup::showCentered },
	{ "shelve"			, &_lua_popup::shelve },
	{ "toggle"			, &_lua_popup::toggle },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_popup>::PropertyType _lua_popup::properties[] = {
	{ "opened"		, wrap( _lua_popup , &_popup::isOpened )		, nullptr },
	{ "owner"		, wrap( _lua_popup , &_popup::getOwner )		, wrap( _lua_popup , &_popup::setOwner ) },
	{ "direction"	, wrap( _lua_popup , &_popup::getDirection )	, nullptr },
	{ "offset"		, wrap( _lua_popup , &_popup::getOffset )		, wrap( _lua_popup , &_popup::setOffset ) },
	LUA_CLASS_ATTR_END
};