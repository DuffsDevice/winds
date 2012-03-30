#include "_lua/lua.class.imagegadget.h"

/*##################################
##           Lua-Window           ##
##################################*/

_lua_imagegadget::_lua_imagegadget( lua_State* L ) : 
	_lua_gadget( new _imagegadget( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , Lunar<_lua_bitmap>::check( L , 3 )->bm ) )
{ }

int _lua_imagegadget::setImage( lua_State* L ){
	((_imagegadget*)_lua_gadget::gadget)->setImage( Lunar<_lua_bitmap>::check( L , 1 )->bm );
	return 0;
}

int _lua_imagegadget::getImage( lua_State* L ){
	Lunar<_lua_bitmap>::push( L , new _lua_bitmap( const_cast<_bitmap*>( ((_imagegadget*)_lua_gadget::gadget)->getImage() ) ) , true );
	return 1;
}

//! Lua-window
const char _lua_imagegadget::className[] = "_imagegadget";

Lunar<_lua_imagegadget>::RegType _lua_imagegadget::methods[] = {
  {"delete", &_lua_imagegadget::_delete},
  LUNAR_DECLARE_METHOD(_lua_imagegadget,bubbleEvent),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,bubbleRefresh),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,refreshBitmap),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,getBitmap),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,getWindows),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,registerEventHandler),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,unregisterEventHandler),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,generateEvent),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,triggerEvent),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,canReactTo),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,handleEvent),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,handleEventDefault),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,getAbsoluteX),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,getAbsoluteY),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,getX),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,getY),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,setX),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,setY),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,moveTo),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,moveRelative),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,getParent),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,setParent),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,removeChild),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,addChild),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,getDimensions),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,getAbsoluteDimensions),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,getHeight),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,setHeight),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,getWidth),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,setWidth),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,standardPaint),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,toDerived),
  
  //! Specific to _imagegadget
  LUNAR_DECLARE_METHOD(_lua_imagegadget,getImage),
  LUNAR_DECLARE_METHOD(_lua_imagegadget,setImage),
  {0,0}
};