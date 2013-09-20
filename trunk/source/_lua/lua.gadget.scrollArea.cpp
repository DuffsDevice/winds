#include "_lua/lua.gadget.scrollArea.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##         Lua-ScrollArea         ##
##################################*/

_lua_scrollarea::_lua_scrollarea( lua_State* L ) :
	_lua_gadget(
		// _scrollArea( _length width , _length height , _coord x , _coord y , _scrollType scrollTypeX = _scrollType::meta, _scrollType scrollTypeY = _scrollType::meta , _style style = _style() );
		new _scrollArea(
			check<int>( L , 1 ) // width
			, check<int>( L , 2 ) // height
			, check<int>( L , 3 ) // x
			, check<int>( L , 4 ) // y
			, string2scrollType[ lightcheck<string>( L , 5 , "meta" ) ] // scrollType
			, string2scrollType[ lightcheck<string>( L , 6 , "meta" ) ] // scrollType
			, lightcheck<_style>( L , 7 , _style() )
		)
	)
{ }

//! setScrollTypeX
int _lua_scrollarea::setScrollTypeX( lua_State* L ){ ((_scrollArea*)_lua_gadget::getGadget())->setScrollTypeX( string2scrollType[ check<string>( L , 1 ) ] ); return 0; };

//! setScrollTypeY
int _lua_scrollarea::setScrollTypeY( lua_State* L ){ ((_scrollArea*)_lua_gadget::getGadget())->setScrollTypeY( string2scrollType[ check<string>( L , 1 ) ] ); return 0; }

//! scrollToX
int _lua_scrollarea::scrollToX( lua_State* L ){ ((_scrollArea*)_lua_gadget::getGadget())->scrollToX( check<int>( L , 1 ) , lightcheck( L , 2 , 1 ) ); return 0; }

//! scrollToY
int _lua_scrollarea::scrollToY( lua_State* L ){ ((_scrollArea*)_lua_gadget::getGadget())->scrollToY( check<int>( L , 1 ) , lightcheck( L , 2 , 1 ) ); return 0; }

//! scrollTo
int _lua_scrollarea::scrollTo( lua_State* L ){ ((_scrollArea*)_lua_gadget::getGadget())->scrollTo( check<int>( L , 1 ) , check<int>( L , 2 ) , lightcheck( L , 3 , 1 ) ); return 0; }

//! getScrollY
int _lua_scrollarea::getScrollY( lua_State* L ){ lua_pushnumber( L , ((_scrollArea*)_lua_gadget::getGadget())->getScrollY() ); return 1; }

//! getScrollX
int _lua_scrollarea::getScrollX( lua_State* L ){ lua_pushnumber( L , ((_scrollArea*)_lua_gadget::getGadget())->getScrollX() ); return 1; }

//! Lua-button
const char _lua_scrollarea::className[] = "ScrollArea";
Lunar<_lua_scrollarea>::FunctionType _lua_scrollarea::methods[] = {
	LUA_CLASS_FUNC(_lua_scrollarea,setScrollTypeX),
	LUA_CLASS_FUNC(_lua_scrollarea,setScrollTypeY),
	LUA_CLASS_FUNC(_lua_scrollarea,scrollToX),
	LUA_CLASS_FUNC(_lua_scrollarea,scrollToY),
	LUA_CLASS_FUNC(_lua_scrollarea,scrollTo),
	LUA_CLASS_FUNC(_lua_scrollarea,getScrollY),
	LUA_CLASS_FUNC(_lua_scrollarea,getScrollX),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_scrollarea>::PropertyType _lua_scrollarea::properties[] = {
	GADGET_BASE_ATTR,
	{ "scrollX" , &_lua_scrollarea::getScrollX , &_lua_scrollarea::scrollToX },
	{ "scrollY" , &_lua_scrollarea::getScrollY , &_lua_scrollarea::scrollToY },
	LUA_CLASS_ATTR_END
};