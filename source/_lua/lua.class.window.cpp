#include "_lua/lua.class.window.h"

/*##################################
##           Lua-Window           ##
##################################*/

_lua_window::_lua_window( lua_State* L ) : 
	_lua_gadget( 
		new _window( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkstring( L , 5 ) )
	)
	, _lua_interface_input( (_window*)_lua_gadget::gadget )
{ }


//! Lua-window
const char _lua_window::className[] = "_window";

Lunar<_lua_window>::RegType _lua_window::methods[] = {
  {"delete", &_lua_window::_delete},
  LUNAR_DECLARE_METHOD(_lua_window,bubbleEvent),
  LUNAR_DECLARE_METHOD(_lua_window,bubbleRefresh),
  LUNAR_DECLARE_METHOD(_lua_window,refreshBitmap),
  LUNAR_DECLARE_METHOD(_lua_window,getBitmap),
  LUNAR_DECLARE_METHOD(_lua_window,getWindows),
  LUNAR_DECLARE_METHOD(_lua_window,registerEventHandler),
  LUNAR_DECLARE_METHOD(_lua_window,unregisterEventHandler),
  LUNAR_DECLARE_METHOD(_lua_window,generateEvent),
  LUNAR_DECLARE_METHOD(_lua_window,triggerEvent),
  LUNAR_DECLARE_METHOD(_lua_window,canReactTo),
  LUNAR_DECLARE_METHOD(_lua_window,handleEvent),
  LUNAR_DECLARE_METHOD(_lua_window,handleEventDefault),
  LUNAR_DECLARE_METHOD(_lua_window,getAbsoluteX),
  LUNAR_DECLARE_METHOD(_lua_window,getAbsoluteY),
  LUNAR_DECLARE_METHOD(_lua_window,getX),
  LUNAR_DECLARE_METHOD(_lua_window,getY),
  LUNAR_DECLARE_METHOD(_lua_window,setX),
  LUNAR_DECLARE_METHOD(_lua_window,setY),
  LUNAR_DECLARE_METHOD(_lua_window,moveTo),
  LUNAR_DECLARE_METHOD(_lua_window,moveRelative),
  LUNAR_DECLARE_METHOD(_lua_window,getParent),
  LUNAR_DECLARE_METHOD(_lua_window,setParent),
  LUNAR_DECLARE_METHOD(_lua_window,removeChild),
  LUNAR_DECLARE_METHOD(_lua_window,addChild),
  LUNAR_DECLARE_METHOD(_lua_window,getDimensions),
  LUNAR_DECLARE_METHOD(_lua_window,getAbsoluteDimensions),
  LUNAR_DECLARE_METHOD(_lua_window,getHeight),
  LUNAR_DECLARE_METHOD(_lua_window,setHeight),
  LUNAR_DECLARE_METHOD(_lua_window,getWidth),
  LUNAR_DECLARE_METHOD(_lua_window,setWidth),
  LUNAR_DECLARE_METHOD(_lua_window,standardPaint),
  LUNAR_DECLARE_METHOD(_lua_window,toDerived),
  
  //! _interface_input (only these two are used)
  LUNAR_DECLARE_METHOD(_lua_window,setTitle),
  LUNAR_DECLARE_METHOD(_lua_window,getTitle),
  {0,0}
};