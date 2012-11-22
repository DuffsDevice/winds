#include "_lua/lua.gadget.scrollArea.h"

/*##################################
##         Lua-ScrollArea         ##
##################################*/

_lua_scrollArea::_lua_scrollArea( lua_State* L )
{ 
	this->setGadget( new _scrollArea( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , string2scrollType[ luaL_checkstring( L , 5 ) ] , string2scrollType[ luaL_checkstring( L , 6 ) ] ) );
}

//! setScrollTypeX
int _lua_scrollArea::setScrollTypeX( lua_State* L ){ ((_scrollArea*)_lua_gadget::gadget)->setScrollTypeX( string2scrollType[ luaL_checkstring( L , 1 ) ] ); return 0; };

//! setScrollTypeY
int _lua_scrollArea::setScrollTypeY( lua_State* L ){ ((_scrollArea*)_lua_gadget::gadget)->setScrollTypeY( string2scrollType[ luaL_checkstring( L , 1 ) ] ); return 0; }

//! setInnerWidth
int _lua_scrollArea::setCanvasWidth( lua_State* L ){ 
	((_scrollArea*)_lua_gadget::gadget)->setCanvasWidth( luaL_optint( L , 1 , 0 ) );
	return 0;
}

//! setInnerHeight
int _lua_scrollArea::setCanvasHeight( lua_State* L ){ 
	((_scrollArea*)_lua_gadget::gadget)->setCanvasHeight( luaL_optint( L , 1 , 0 ) );
	return 0;
}

//! scrollToX
int _lua_scrollArea::scrollToX( lua_State* L ){ ((_scrollArea*)_lua_gadget::gadget)->scrollToX( luaL_checkint( L , 1 ) , luaL_optint( L , 2 , 1 ) ); return 0; }

//! scrollToY
int _lua_scrollArea::scrollToY( lua_State* L ){ ((_scrollArea*)_lua_gadget::gadget)->scrollToY( luaL_checkint( L , 1 ) , luaL_optint( L , 2 , 1 ) ); return 0; }

//! scrollTo
int _lua_scrollArea::scrollTo( lua_State* L ){ ((_scrollArea*)_lua_gadget::gadget)->scrollTo( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_optint( L , 3 , 1 ) ); return 0; }

//! getScrollY
int _lua_scrollArea::getScrollY( lua_State* L ){ lua_pushnumber( L , ((_scrollArea*)_lua_gadget::gadget)->getScrollY() ); return 1; }

//! getScrollX
int _lua_scrollArea::getScrollX( lua_State* L ){ lua_pushnumber( L , ((_scrollArea*)_lua_gadget::gadget)->getScrollX() ); return 1; }

//! Lua-button
const char _lua_scrollArea::className[] = "_scrollArea";
Lunar<_lua_scrollArea>::FunctionType _lua_scrollArea::methods[] = {
	GADGET_FUNCS( _lua_scrollArea ),
	
	//! _interface_input (only these two are used)
	LUA_CLASS_FUNC(_lua_scrollArea,setScrollTypeX),
	LUA_CLASS_FUNC(_lua_scrollArea,setScrollTypeY),
	LUA_CLASS_FUNC(_lua_scrollArea,setCanvasWidth),
	LUA_CLASS_FUNC(_lua_scrollArea,setCanvasHeight),
	LUA_CLASS_FUNC(_lua_scrollArea,scrollToX),
	LUA_CLASS_FUNC(_lua_scrollArea,scrollToY),
	LUA_CLASS_FUNC(_lua_scrollArea,scrollTo),
	LUA_CLASS_FUNC(_lua_scrollArea,getScrollY),
	LUA_CLASS_FUNC(_lua_scrollArea,getScrollX),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_scrollArea>::PropertyType _lua_scrollArea::properties[] = {
	GADGET_ATTRS( _lua_scrollArea ),
	LUA_CLASS_ATTR_END
};