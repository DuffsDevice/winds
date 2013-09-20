#include "_lua/lua.gadget.stickybutton.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##         Lua-Stickybutton       ##
##################################*/

_lua_stickybutton::_lua_stickybutton( lua_State* L )
{
	if( lua_gettop( L ) <= 4 && lua_isstring( L , 3 ) )
		// _stickybutton( _coord x , _coord y , string title = "" , _style style = _style() );
		this->setGadget( new _stickybutton( check<int>( L , 1 ) , check<int>( L , 2 ) , lightcheck<string>( L , 3 , "" ) , lightcheck<_style>( L , 4 , _style() ) ) );
	else
		// _stickybutton( _length width , _length height , _coord x , _coord y , string title = "" , _style style = _style() );
		this->setGadget( new _stickybutton( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) , check<int>( L , 4 ) , lightcheck<string>( L , 5 , "" ) , lightcheck<_style>( L , 6 , _style() ) ) );
	
	_lua_interface_input::input = (_stickybutton*)_lua_gadget::getGadget();
}

//! setAutoSelect
int _lua_stickybutton::setAutoSelect( lua_State* L ){ _lua_interface_input::input->setAutoSelect( check<bool>( L , 1 ) ); return 0; };

//! isAutoSelect
int _lua_stickybutton::isAutoSelect( lua_State* L ){ lua_pushboolean( L , _lua_interface_input::input->isAutoSelect() ); return 1; }


//! Lua-button
const char _lua_stickybutton::className[] = "StickyButton";
Lunar<_lua_stickybutton>::FunctionType _lua_stickybutton::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_stickybutton>::PropertyType _lua_stickybutton::properties[] = {
	GADGET_BASE_ATTR,
	{ "checked" , &_lua_stickybutton::getIntValue , &_lua_stickybutton::setIntValue },
	{ "text" , &_lua_stickybutton::getStrValue , &_lua_stickybutton::setStrValue },
	{ "autoSelect" , &_lua_stickybutton::isAutoSelect , &_lua_stickybutton::setAutoSelect },
	{ "font" , &_lua_stickybutton::getFont , &_lua_stickybutton::setFont },
	{ "fontSize" , &_lua_stickybutton::getFontSize , &_lua_stickybutton::setFontSize },
	LUA_CLASS_ATTR_END
};