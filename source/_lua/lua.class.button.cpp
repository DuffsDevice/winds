#include "_lua/lua.class.button.h"

/*##################################
##           Lua-Button           ##
##################################*/

_lua_button::_lua_button( lua_State* L ) : 
	_lua_gadget( (_gadget*)nullptr ) , _lua_interface_input( (_interface_input*)nullptr )
{ 
	if( lua_gettop( L ) == 3 )
		_lua_gadget::gadget = new _button( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkstring( L , 3 ) );
	else
		_lua_gadget::gadget = new _button( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkstring( L , 5 ) );
	
	_lua_interface_input::input = (_button*)_lua_gadget::gadget;
}

//! setAutoSelect
int _lua_button::setAutoSelect( lua_State* L ){ ((_button*)_lua_gadget::gadget)->setAutoSelect( luaL_optint( L , 1 , true ) ); return 0; };

//! isAutoSelect
int _lua_button::isAutoSelect( lua_State* L ){ lua_pushboolean( L , ((_button*)_lua_gadget::gadget)->isAutoSelect() ); return 1; }

//! Lua-button
const char _lua_button::className[] = "_button";
Lunar<_lua_button>::RegType _lua_button::methods[] = {
  {"delete", &_lua_button::_delete},
  LUNAR_DECLARE_METHOD(_lua_button,bubbleEvent),
  LUNAR_DECLARE_METHOD(_lua_button,bubbleRefresh),
  LUNAR_DECLARE_METHOD(_lua_button,refreshBitmap),
  LUNAR_DECLARE_METHOD(_lua_button,getBitmap),
  LUNAR_DECLARE_METHOD(_lua_button,getWindows),
  LUNAR_DECLARE_METHOD(_lua_button,registerEventHandler),
  LUNAR_DECLARE_METHOD(_lua_button,unregisterEventHandler),
  LUNAR_DECLARE_METHOD(_lua_button,generateEvent),
  LUNAR_DECLARE_METHOD(_lua_button,triggerEvent),
  LUNAR_DECLARE_METHOD(_lua_button,canReactTo),
  LUNAR_DECLARE_METHOD(_lua_button,handleEvent),
  LUNAR_DECLARE_METHOD(_lua_button,handleEventDefault),
  LUNAR_DECLARE_METHOD(_lua_button,getAbsoluteX),
  LUNAR_DECLARE_METHOD(_lua_button,getAbsoluteY),
  LUNAR_DECLARE_METHOD(_lua_button,getX),
  LUNAR_DECLARE_METHOD(_lua_button,getY),
  LUNAR_DECLARE_METHOD(_lua_button,setX),
  LUNAR_DECLARE_METHOD(_lua_button,setY),
  LUNAR_DECLARE_METHOD(_lua_button,moveTo),
  LUNAR_DECLARE_METHOD(_lua_button,moveRelative),
  LUNAR_DECLARE_METHOD(_lua_button,getParent),
  LUNAR_DECLARE_METHOD(_lua_button,setParent),
  LUNAR_DECLARE_METHOD(_lua_button,removeChild),
  LUNAR_DECLARE_METHOD(_lua_button,addChild),
  LUNAR_DECLARE_METHOD(_lua_button,getDimensions),
  LUNAR_DECLARE_METHOD(_lua_button,getAbsoluteDimensions),
  LUNAR_DECLARE_METHOD(_lua_button,getHeight),
  LUNAR_DECLARE_METHOD(_lua_button,setHeight),
  LUNAR_DECLARE_METHOD(_lua_button,getWidth),
  LUNAR_DECLARE_METHOD(_lua_button,setWidth),
  LUNAR_DECLARE_METHOD(_lua_button,standardPaint),
  LUNAR_DECLARE_METHOD(_lua_button,toDerived),
  
  //! _interface_input (only these two are used)
  LUNAR_DECLARE_METHOD(_lua_button,setTitle),
  LUNAR_DECLARE_METHOD(_lua_button,getTitle),
  LUNAR_DECLARE_METHOD(_lua_button,setAutoSelect),
  LUNAR_DECLARE_METHOD(_lua_button,isAutoSelect),
  {0,0}
};