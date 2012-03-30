#include "_lua/lua.class.label.h"

/*##################################
##            Lua-Label           ##
##################################*/

_lua_label::_lua_label( lua_State* L ) : 
	_lua_gadget( 
		new _label( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkstring( L , 3 ) )
	)
	, _lua_interface_input( (_label*)_lua_gadget::gadget )
{ }


//! Lua-window
const char _lua_label::className[] = "_label";

Lunar<_lua_label>::RegType _lua_label::methods[] = {
  {"delete", &_lua_label::_delete},
  LUNAR_DECLARE_METHOD(_lua_label,bubbleEvent),
  LUNAR_DECLARE_METHOD(_lua_label,bubbleRefresh),
  LUNAR_DECLARE_METHOD(_lua_label,refreshBitmap),
  LUNAR_DECLARE_METHOD(_lua_label,getBitmap),
  LUNAR_DECLARE_METHOD(_lua_label,getWindows),
  LUNAR_DECLARE_METHOD(_lua_label,registerEventHandler),
  LUNAR_DECLARE_METHOD(_lua_label,unregisterEventHandler),
  LUNAR_DECLARE_METHOD(_lua_label,generateEvent),
  LUNAR_DECLARE_METHOD(_lua_label,triggerEvent),
  LUNAR_DECLARE_METHOD(_lua_label,canReactTo),
  LUNAR_DECLARE_METHOD(_lua_label,handleEvent),
  LUNAR_DECLARE_METHOD(_lua_label,handleEventDefault),
  LUNAR_DECLARE_METHOD(_lua_label,getAbsoluteX),
  LUNAR_DECLARE_METHOD(_lua_label,getAbsoluteY),
  LUNAR_DECLARE_METHOD(_lua_label,getX),
  LUNAR_DECLARE_METHOD(_lua_label,getY),
  LUNAR_DECLARE_METHOD(_lua_label,setX),
  LUNAR_DECLARE_METHOD(_lua_label,setY),
  LUNAR_DECLARE_METHOD(_lua_label,moveTo),
  LUNAR_DECLARE_METHOD(_lua_label,moveRelative),
  LUNAR_DECLARE_METHOD(_lua_label,getParent),
  LUNAR_DECLARE_METHOD(_lua_label,setParent),
  LUNAR_DECLARE_METHOD(_lua_label,removeChild),
  LUNAR_DECLARE_METHOD(_lua_label,addChild),
  LUNAR_DECLARE_METHOD(_lua_label,getDimensions),
  LUNAR_DECLARE_METHOD(_lua_label,getAbsoluteDimensions),
  LUNAR_DECLARE_METHOD(_lua_label,getHeight),
  LUNAR_DECLARE_METHOD(_lua_label,setHeight),
  LUNAR_DECLARE_METHOD(_lua_label,getWidth),
  LUNAR_DECLARE_METHOD(_lua_label,setWidth),
  LUNAR_DECLARE_METHOD(_lua_label,standardPaint),
  LUNAR_DECLARE_METHOD(_lua_label,toDerived),
  
  //! _interface_input (only these two are used)
  LUNAR_DECLARE_METHOD(_lua_label,getStrValue),
  LUNAR_DECLARE_METHOD(_lua_label,setStrValue),
  {0,0}
};