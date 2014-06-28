#include "_lua/lua.gadget.label.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##            Lua-Label           ##
##################################*/

_lua_label::_lua_label( lua_State* L ) :
	_lua_gadget( new _label( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , optcheck<int>( L , 3 ) , optcheck<int>( L , 4 ) , check<string>( L , 5 ) , lightcheck<_style>( L , 6 , _style() ) ) )
{ }

int _lua_label::setEllipsis( lua_State* L ){
	this->getGadget<_label>()->setEllipsis( check<bool>( L , 1 ) , lightcheck<_letterNum>( L , 2 ) );
	return 0;
}

//! Lua-window
const char _lua_label::className[] = "Label";
Lunar<_lua_label>::FunctionType _lua_label::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_label>::PropertyType _lua_label::properties[] = {
	{ "text" 		, wrap( _lua_label , &_label::getStrValue )	, wrap( _lua_label , &_label::setStrValue ) },
	{ "color" 		, wrap( _lua_label , &_label::getColor )	, wrap( _lua_label , &_label::setColor ) },
	{ "bgColor" 	, wrap( _lua_label , &_label::getBgColor )	, wrap( _lua_label , &_label::setBgColor ) },
	{ "font" 		, wrap( _lua_label , &_label::getFont )		, wrap( _lua_label , &_label::setFont ) },
	{ "fontSize"	, wrap( _lua_label , &_label::getFontSize )	, wrap( _lua_label , &_label::setFontSize ) },
	{ "align"		, wrap( _lua_label , &_label::getAlign )	, wrap( _lua_label , &_label::setAlign ) },
	{ "vAlign"		, wrap( _lua_label , &_label::getVAlign )	, wrap( _lua_label , &_label::setVAlign ) },
	{ "ellipsis"	, wrap( _lua_label , &_label::getEllipsis ) , &_lua_label::setEllipsis },
	LUA_CLASS_ATTR_END
};