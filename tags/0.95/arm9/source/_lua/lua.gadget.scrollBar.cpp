#include "_lua/lua.gadget.scrollBar.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##         Lua-ScrollBar          ##
##################################*/

_lua_scrollbar::_lua_scrollbar( lua_State* L ) :
	_lua_gadget(
		new _scrollBar(
			optcheck<int>( L , 1 ) // x
			, optcheck<int>( L , 2 ) // y
			, optcheck<int>( L , 3 ) // gadgetLength
			, check<int>( L , 4 ) // length
			, check<int>( L , 5 ) // length2
			, check<_dimension>( L , 4 ) // dim
			, lightcheck<int>( L , 7 , 0 ) // // value
			, lightcheck<_style>( L , 8 )
		)
	)
{}

//! Lua-button
const char _lua_scrollbar::className[] = "ScrollBar";
Lunar<_lua_scrollbar>::FunctionType _lua_scrollbar::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_scrollbar>::PropertyType _lua_scrollbar::properties[] = {
	{ "step"		, wrap( _lua_scrollbar , &_scrollBar::getStep ) , wrap( _lua_scrollbar , &_scrollBar::setStep )  },
	{ "length"		, wrap( _lua_scrollbar , &_scrollBar::getLength ) , wrap( _lua_scrollbar , &_scrollBar::setLength )  },
	{ "length2"		, wrap( _lua_scrollbar , &_scrollBar::getLength2 ) , wrap( _lua_scrollbar , &_scrollBar::setLength2 )  },
	{ "value"		, wrap( _lua_scrollbar , &_scrollBar::getValue ), wrap( _lua_scrollbar , &_scrollBar::setValue )  },
	{ "dimension"	, wrap( _lua_scrollbar , &_scrollBar::getDimension ) , wrap( _lua_scrollbar , &_scrollBar::setDimension )  },
	LUA_CLASS_ATTR_END
};