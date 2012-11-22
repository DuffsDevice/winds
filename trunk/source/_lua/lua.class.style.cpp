#include "_lua/lua.class.style.h"
#include "_lua/lua.class.gadget.h"

/*##################################
##            Lua-Style            ##
##################################*/

/*_lua_style::_lua_style( _rect rc ) : _rect( rc )
{}

//! Constructor
_lua_style::_lua_style( lua_State* L ) : 
	_rect( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) )
{ }
_lua_style( _style st );

//! Constructor
_lua_style( lua_State* L );

//! owner
int _lua_style::setOwner(lua_State* L){ _style::owner = luaL_checkint( );return 0;
int _lua_style::getOwner(lua_State* L);

//! data
int _lua_style::setData(lua_State* L);
int _lua_style::getData(lua_State* L);

//! resizable
int _lua_style::setResizable(lua_State* L);
int _lua_style::getResizable(lua_State* L);

//! destroyable
int _lua_style::setDestroyable(lua_State* L);
int _lua_style::getDestroyable(lua_State* L);

//! focused
int _lua_style::getFocused(lua_State* L);

//! pressed
int _lua_style::getPressed(lua_State* L);

//! enhanced
int _lua_style::getEnhanced(lua_State* L);

//! dragged
int _lua_style::getDragged(lua_State* L);

//! draggable
int _lua_style::setDraggable(lua_State* L);
int _lua_style::getDraggable(lua_State* L);

//! editable
int _lua_style::setEditable(lua_State* L);
int _lua_style::getEditable(lua_State* L);

//! showsUpInTaskBar
int _lua_style::setShowsUpInTaskBar(lua_State* L);
int _lua_style::getShowsUpInTaskBar(lua_State* L);

//! canTakeFocus
int _lua_style::setCanTakeFocus(lua_State* L);
int _lua_style::getCanTakeFocus(lua_State* L);

//! canLooseFocus
int _lua_style::setCanLooseFocus(lua_State* L);
int _lua_style::getCanLooseFocus(lua_State* L);

//! canReceiveFocus
int _lua_style::setCanReceiveFocus(lua_State* L);
int _lua_style::getCanReceiveFocus(lua_State* L);

//! smallDragTrig
int _lua_style::setSmallDragTrig(lua_State* L);
int _lua_style::getSmallDragTrig(lua_State* L);

//! Lua-_rect
const char _lua_style::className[] = "_rect";
Lunar<_lua_style>::FunctionType _lua_style::methods[] = {
  LUA_CLASS_FUNC(_lua_style, setX),
  LUA_CLASS_FUNC(_lua_style, setY),
  LUA_CLASS_FUNC(_lua_style, getX),
  LUA_CLASS_FUNC(_lua_style, getY),
  LUA_CLASS_FUNC(_lua_style, setX2),
  LUA_CLASS_FUNC(_lua_style, setY2),
  LUA_CLASS_FUNC(_lua_style, getX2),
  LUA_CLASS_FUNC(_lua_style, getY2),
  LUA_CLASS_FUNC(_lua_style, setWidth),
  LUA_CLASS_FUNC(_lua_style, setHeight),
  LUA_CLASS_FUNC(_lua_style, getWidth),
  LUA_CLASS_FUNC(_lua_style, getHeight),
  LUA_CLASS_FUNC(_lua_style, isValid),
  LUA_CLASS_FUNC(_lua_style, contains),
  LUA_CLASS_FUNC(_lua_style, fromCoords),
  LUA_CLASS_FUNC(_lua_style, toRelative),
  LUA_CLASS_FUNC(_lua_style, clipToIntersect),
  LUA_CLASS_FUNC(_lua_style, expandToInclude),
  LUA_CLASS_FUNC(_lua_style, reduce),
  LUA_CLASS_FUNC(_lua_style, combine),
  LUA_CLASS_FUNC(_lua_style, intersectsWith),
  LUA_CLASS_FUNC_END
};

Lunar<_lua_style>::PropertyType _lua_style::properties[] = {
	LUA_CLASS_ATTR_END
};*/