#include "_lua/lua.gadget.scrollArea.h"

/*##################################
##         Lua-ScrollArea         ##
##################################*/

_lua_scrollarea::_lua_scrollarea( lua_State* L ) :
	_lua_gadget(
		// _scrollArea( _length width , _length height , _coord x , _coord y , _scrollType scrollTypeX = _scrollType::meta, _scrollType scrollTypeY = _scrollType::meta , _style style = _style() );
		new _scrollArea(
			luaL_checkint( L , 1 ) // width
			, luaL_checkint( L , 2 ) // height
			, luaL_checkint( L , 3 ) // x
			, luaL_checkint( L , 4 ) // y
			, string2scrollType[ luaL_optstring( L , 5 , "meta" ) ] // scrollType
			, string2scrollType[ luaL_optstring( L , 6 , "meta" ) ] // scrollType
			, luaL_optstyle( L , 7 )
		)
	)
{ }

//! setScrollTypeX
int _lua_scrollarea::setScrollTypeX( lua_State* L ){ ((_scrollArea*)_lua_gadget::gadget)->setScrollTypeX( string2scrollType[ luaL_checkstring( L , 1 ) ] ); return 0; };

//! setScrollTypeY
int _lua_scrollarea::setScrollTypeY( lua_State* L ){ ((_scrollArea*)_lua_gadget::gadget)->setScrollTypeY( string2scrollType[ luaL_checkstring( L , 1 ) ] ); return 0; }

//! setInnerWidth
int _lua_scrollarea::setCanvasWidth( lua_State* L ){ 
	((_scrollArea*)_lua_gadget::gadget)->setCanvasWidth( luaL_optint( L , 1 , 0 ) );
	return 0;
}

//! setInnerHeight
int _lua_scrollarea::setCanvasHeight( lua_State* L ){ 
	((_scrollArea*)_lua_gadget::gadget)->setCanvasHeight( luaL_optint( L , 1 , 0 ) );
	return 0;
}

//! scrollToX
int _lua_scrollarea::scrollToX( lua_State* L ){ ((_scrollArea*)_lua_gadget::gadget)->scrollToX( luaL_checkint( L , 1 ) , luaL_optint( L , 2 , 1 ) ); return 0; }

//! scrollToY
int _lua_scrollarea::scrollToY( lua_State* L ){ ((_scrollArea*)_lua_gadget::gadget)->scrollToY( luaL_checkint( L , 1 ) , luaL_optint( L , 2 , 1 ) ); return 0; }

//! scrollTo
int _lua_scrollarea::scrollTo( lua_State* L ){ ((_scrollArea*)_lua_gadget::gadget)->scrollTo( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_optint( L , 3 , 1 ) ); return 0; }

//! getScrollY
int _lua_scrollarea::getScrollY( lua_State* L ){ lua_pushnumber( L , ((_scrollArea*)_lua_gadget::gadget)->getScrollY() ); return 1; }

//! getScrollX
int _lua_scrollarea::getScrollX( lua_State* L ){ lua_pushnumber( L , ((_scrollArea*)_lua_gadget::gadget)->getScrollX() ); return 1; }

//! Lua-button
const char _lua_scrollarea::className[] = "_scrollArea";
Lunar<_lua_scrollarea>::FunctionType _lua_scrollarea::methods[] = {
	GADGET_FUNCS( _lua_scrollarea ),
	
	//! _interface_input (only these two are used)
	LUA_CLASS_FUNC(_lua_scrollarea,setScrollTypeX),
	LUA_CLASS_FUNC(_lua_scrollarea,setScrollTypeY),
	LUA_CLASS_FUNC(_lua_scrollarea,setCanvasWidth),
	LUA_CLASS_FUNC(_lua_scrollarea,setCanvasHeight),
	LUA_CLASS_FUNC(_lua_scrollarea,scrollToX),
	LUA_CLASS_FUNC(_lua_scrollarea,scrollToY),
	LUA_CLASS_FUNC(_lua_scrollarea,scrollTo),
	LUA_CLASS_FUNC(_lua_scrollarea,getScrollY),
	LUA_CLASS_FUNC(_lua_scrollarea,getScrollX),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_scrollarea>::PropertyType _lua_scrollarea::properties[] = {
	GADGET_ATTRS( _lua_scrollarea ),
	{ "scrollX" , &_lua_scrollarea::getScrollX , &_lua_scrollarea::scrollToX },
	{ "scrollY" , &_lua_scrollarea::getScrollY , &_lua_scrollarea::scrollToY },
	LUA_CLASS_ATTR_END
};