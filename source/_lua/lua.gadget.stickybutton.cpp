#include "_lua/lua.gadget.stickybutton.h"

/*##################################
##         Lua-Stickybutton       ##
##################################*/

_lua_stickybutton::_lua_stickybutton( lua_State* L )
{
	if( lua_gettop( L ) <= 4 && lua_isstring( L , 3 ) )
		// _stickybutton( _coord x , _coord y , string title = "" , _style style = _style() );
		this->setGadget( new _stickybutton( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_optstring( L , 3 , "" ) , luaL_optstyle( L , 4 ) ) );
	else
		// _stickybutton( _length width , _length height , _coord x , _coord y , string title = "" , _style style = _style() );
		this->setGadget( new _stickybutton( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_optstring( L , 5 , "" ) , luaL_optstyle( L , 6 ) ) );
	
	_lua_interface_input::input = (_stickybutton*)_lua_gadget::getGadget();
}

//! setAutoSelect
int _lua_stickybutton::setAutoSelect( lua_State* L ){	_lua_interface_input::input->setAutoSelect( luaL_checkboolean( L , 1 ) ); return 0; };

//! isAutoSelect
int _lua_stickybutton::isAutoSelect( lua_State* L ){ lua_pushboolean( L , _lua_interface_input::input->isAutoSelect() ); return 1; }


//! Lua-button
const char _lua_stickybutton::className[] = "_stickybutton";
Lunar<_lua_stickybutton>::FunctionType _lua_stickybutton::methods[] = {
	GADGET_FUNCS( _lua_stickybutton ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_stickybutton>::PropertyType _lua_stickybutton::properties[] = {
	GADGET_ATTRS( _lua_stickybutton ),
	{ "checked" , &_lua_stickybutton::getIntValue , &_lua_stickybutton::setIntValue },
	{ "text" , &_lua_stickybutton::getStrValue , &_lua_stickybutton::setStrValue },
	{ "autoSelect" , &_lua_stickybutton::isAutoSelect , &_lua_stickybutton::setAutoSelect },
	{ "font" , &_lua_stickybutton::getFont , &_lua_stickybutton::setFont },
	{ "fontSize" , &_lua_stickybutton::getFontSize , &_lua_stickybutton::setFontSize },
	LUA_CLASS_ATTR_END
};