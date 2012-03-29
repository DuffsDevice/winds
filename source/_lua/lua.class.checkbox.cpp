#include "_lua/lua.class.checkbox.h"

/*##################################
##           Lua-Window           ##
##################################*/

_lua_checkbox::_lua_checkbox( lua_State* L ) : 
	_lua_gadget( 
		new _checkbox( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) )
	)
	, _lua_interface_input( (_checkbox*)_lua_gadget::gadget )
{ }


//! Lua-window
const char _lua_checkbox::className[] = "_checkbox";

Lunar<_lua_checkbox>::RegType _lua_checkbox::methods[] = {
  {"delete", &_lua_checkbox::_delete},
  LUNAR_DECLARE_METHOD(_lua_checkbox,bubbleEvent),
  LUNAR_DECLARE_METHOD(_lua_checkbox,bubbleRefresh),
  LUNAR_DECLARE_METHOD(_lua_checkbox,bubbleRealRefresh),
  LUNAR_DECLARE_METHOD(_lua_checkbox,refreshBitmap),
  LUNAR_DECLARE_METHOD(_lua_checkbox,getBitmap),
  LUNAR_DECLARE_METHOD(_lua_checkbox,getWindows),
  LUNAR_DECLARE_METHOD(_lua_checkbox,registerEventHandler),
  LUNAR_DECLARE_METHOD(_lua_checkbox,unregisterEventHandler),
  LUNAR_DECLARE_METHOD(_lua_checkbox,generateEvent),
  LUNAR_DECLARE_METHOD(_lua_checkbox,triggerEvent),
  LUNAR_DECLARE_METHOD(_lua_checkbox,canReactTo),
  LUNAR_DECLARE_METHOD(_lua_checkbox,handleEvent),
  LUNAR_DECLARE_METHOD(_lua_checkbox,handleEventDefault),
  LUNAR_DECLARE_METHOD(_lua_checkbox,getAbsoluteX),
  LUNAR_DECLARE_METHOD(_lua_checkbox,getAbsoluteY),
  LUNAR_DECLARE_METHOD(_lua_checkbox,getX),
  LUNAR_DECLARE_METHOD(_lua_checkbox,getY),
  LUNAR_DECLARE_METHOD(_lua_checkbox,setX),
  LUNAR_DECLARE_METHOD(_lua_checkbox,setY),
  LUNAR_DECLARE_METHOD(_lua_checkbox,moveTo),
  LUNAR_DECLARE_METHOD(_lua_checkbox,moveRelative),
  LUNAR_DECLARE_METHOD(_lua_checkbox,getParent),
  LUNAR_DECLARE_METHOD(_lua_checkbox,setParent),
  LUNAR_DECLARE_METHOD(_lua_checkbox,removeChild),
  LUNAR_DECLARE_METHOD(_lua_checkbox,addChild),
  LUNAR_DECLARE_METHOD(_lua_checkbox,getDimensions),
  LUNAR_DECLARE_METHOD(_lua_checkbox,getAbsoluteDimensions),
  LUNAR_DECLARE_METHOD(_lua_checkbox,getHeight),
  LUNAR_DECLARE_METHOD(_lua_checkbox,setHeight),
  LUNAR_DECLARE_METHOD(_lua_checkbox,getWidth),
  LUNAR_DECLARE_METHOD(_lua_checkbox,setWidth),
  LUNAR_DECLARE_METHOD(_lua_checkbox,standardPaint),
  LUNAR_DECLARE_METHOD(_lua_checkbox,toDerived),
  
  //! _interface_input (only these two are used)
  LUNAR_DECLARE_METHOD(_lua_checkbox,setTitle),
  LUNAR_DECLARE_METHOD(_lua_checkbox,getTitle),
  LUNAR_DECLARE_METHOD(_lua_checkbox,getIntValue),
  LUNAR_DECLARE_METHOD(_lua_checkbox,setIntValue),
  {0,0}
};