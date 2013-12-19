#include "_lua/lua.gadget.scrollArea.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##         Lua-ScrollArea         ##
##################################*/

_lua_scrollarea::_lua_scrollarea( lua_State* L ) :
	_lua_gadget(
		new _scrollArea(
			optcheck<int>( L , 1 ) // width
			, optcheck<int>( L , 2 ) // height
			, optcheck<int>( L , 3 ) // x
			, optcheck<int>( L , 4 ) // y
			, string2scrollType[ lightcheck<string>( L , 5 , "meta" ) ] // scrollType
			, string2scrollType[ lightcheck<string>( L , 6 , "meta" ) ] // scrollType
			, lightcheck<_style>( L , 7 )
		)
	)
{ }

//! setScrollTypeX
int _lua_scrollarea::setScrollTypeX( lua_State* L ){ ((_scrollArea*)_lua_gadget::getGadget())->setScrollTypeX( string2scrollType[ check<string>( L , 1 ) ] ); return 0; }

//! setScrollTypeY
int _lua_scrollarea::setScrollTypeY( lua_State* L ){ ((_scrollArea*)_lua_gadget::getGadget())->setScrollTypeY( string2scrollType[ check<string>( L , 1 ) ] ); return 0; }

//! Lua-button
const char _lua_scrollarea::className[] = "ScrollArea";
Lunar<_lua_scrollarea>::FunctionType _lua_scrollarea::methods[] = {
	LUA_CLASS_FUNC(_lua_scrollarea,setScrollTypeX),
	LUA_CLASS_FUNC(_lua_scrollarea,setScrollTypeY),
	{ "scrollToX" , wrap( _lua_scrollarea , &_scrollArea::scrollToX ) },
	{ "scrollToY" , wrap( _lua_scrollarea , &_scrollArea::scrollToY ) },
	{ "scrollTo" , wrap( _lua_scrollarea , &_scrollArea::scrollTo ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_scrollarea>::PropertyType _lua_scrollarea::properties[] = {
	GADGET_BASE_ATTR,
	{ "scrollX"	, wrap( _lua_scrollarea , &_scrollArea::getScrollY ) , wrap( _lua_scrollarea , &_scrollArea::scrollToX ) },
	{ "scrollY"	, wrap( _lua_scrollarea , &_scrollArea::getScrollY ) , wrap( _lua_scrollarea , &_scrollArea::scrollToY ) },
	LUA_CLASS_ATTR_END
};