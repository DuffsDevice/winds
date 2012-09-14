#include "_lua/lua.class.rect.h"


/*##################################
##            Lua-Rect            ##
##################################*/

_lua_rect::_lua_rect( _rect rc ) : _rect( rc )
{}

//! Constructor
_lua_rect::_lua_rect( lua_State* L ) : 
	_rect( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) )
{ }
//! Set X
int _lua_rect::setX(lua_State* L){ _rect::x = luaL_checkint( L , 1 ); return 0; }
//! Set Y
int _lua_rect::setY(lua_State* L){ _rect::y = luaL_checkint( L , 1 ); return 0; }
//! Get X
int _lua_rect::getX(lua_State* L){ lua_pushnumber( L , _rect::x ); return 1; }
//! Get Y
int _lua_rect::getY(lua_State* L){ lua_pushnumber( L , _rect::y ); return 1; }
//! Set X2
int _lua_rect::setX2(lua_State* L){ _rect::setX2( luaL_checkint( L , 1 ) ); return 0; }
//! Set Y2
int _lua_rect::setY2(lua_State* L){ _rect::setY2( luaL_checkint( L , 1 ) ); return 0; }
//! Get X2
int _lua_rect::getX2(lua_State* L){ lua_pushnumber( L , _rect::getX2() ); return 1; }
//! Get Y2
int _lua_rect::getY2(lua_State* L){ lua_pushnumber( L , _rect::getY2() ); return 1; }
//! Set Width
int _lua_rect::setWidth(lua_State* L){ _rect::width = luaL_checkint( L , 1 ); return 0; }
//! Set Height
int _lua_rect::setHeight(lua_State* L){ _rect::height = luaL_checkint( L , 1 ); return 0; }
//! Get Width
int _lua_rect::getWidth(lua_State* L){ lua_pushnumber( L , _rect::width ); return 1; }
//! Get Height
int _lua_rect::getHeight(lua_State* L){ lua_pushnumber( L , _rect::height ); return 1; }
//! isValid
int _lua_rect::isValid(lua_State* L){ lua_pushboolean( L , _rect::isValid() ); return 1; }
//! Contains (Coords)
int _lua_rect::contains(lua_State* L){
	if( lua_gettop(L) == 2 )
		lua_pushboolean( L , _rect::contains( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) ) );
	else{
		_lua_rect* rc = Lunar<_lua_rect>::check( L , 1 );
		if( !rc ) 
			return 0;
		lua_pushboolean( L , _rect::contains( *rc ) );
	}
	return 1; 
}
//! fromCoords
int _lua_rect::fromCoords(lua_State* L){
	Lunar<_lua_rect>::push( L , new _lua_rect( _rect::fromCoords( 
		luaL_checkint( L , 1 ),
		luaL_checkint( L , 2 ),
		luaL_checkint( L , 3 ),
		luaL_checkint( L , 4 )
	) ) , true ); 
	return 1;
}
//! toRelative
int _lua_rect::toRelative(lua_State* L){ _rect::toRelative( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) ); return 0; }
//! AND
int _lua_rect::clipToIntersect(lua_State* L){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( !rc ) return 0; _rect::clipToIntersect( *rc ); return 0; }
//! +
int _lua_rect::expandToInclude(lua_State* L){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( !rc ) return 0; _rect::expandToInclude( *rc ); return 0; }
//! -
int _lua_rect::reduce(lua_State* L){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( !rc ) return 0; Lunar<_lua_area>::push( L , new _lua_area( _rect::reduce( *rc ) ) , true ); return 1; }
//! OR
int _lua_rect::combine(lua_State* L){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( !rc ) return 0; Lunar<_lua_area>::push( L , new _lua_area( _rect::combine( *rc ) ) , true ); return 1; }
//! intersectsWith
int _lua_rect::intersectsWith(lua_State* L){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( !rc ) return 0; lua_pushboolean( L , _rect::intersectsWith( *rc ) ); return 1; }

//! Lua-_rect
const char _lua_rect::className[] = "_rect";
Lunar<_lua_rect>::RegType _lua_rect::methods[] = {
  LUNAR_DECLARE_METHOD(_lua_rect, setX),
  LUNAR_DECLARE_METHOD(_lua_rect, setY),
  LUNAR_DECLARE_METHOD(_lua_rect, getX),
  LUNAR_DECLARE_METHOD(_lua_rect, getY),
  LUNAR_DECLARE_METHOD(_lua_rect, setX2),
  LUNAR_DECLARE_METHOD(_lua_rect, setY2),
  LUNAR_DECLARE_METHOD(_lua_rect, getX2),
  LUNAR_DECLARE_METHOD(_lua_rect, getY2),
  LUNAR_DECLARE_METHOD(_lua_rect, setWidth),
  LUNAR_DECLARE_METHOD(_lua_rect, setHeight),
  LUNAR_DECLARE_METHOD(_lua_rect, getWidth),
  LUNAR_DECLARE_METHOD(_lua_rect, getHeight),
  LUNAR_DECLARE_METHOD(_lua_rect, isValid),
  LUNAR_DECLARE_METHOD(_lua_rect, contains),
  LUNAR_DECLARE_METHOD(_lua_rect, fromCoords),
  LUNAR_DECLARE_METHOD(_lua_rect, toRelative),
  LUNAR_DECLARE_METHOD(_lua_rect, clipToIntersect),
  LUNAR_DECLARE_METHOD(_lua_rect, expandToInclude),
  LUNAR_DECLARE_METHOD(_lua_rect, reduce),
  LUNAR_DECLARE_METHOD(_lua_rect, combine),
  LUNAR_DECLARE_METHOD(_lua_rect, intersectsWith),
  {0,0}
};




/*##################################
##            Lua-Area            ##
##################################*/

_lua_area::_lua_area( _area a ) : _area( a )
{ }

//! Constructor
_lua_area::_lua_area( lua_State* L ) : _area() { }
//! Push-back
int _lua_area::add( lua_State* L ){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( rc ) _area::add( (const _rect&)*rc ); return 0; }
//! Push-back
int _lua_area::reduce( lua_State* L ){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( rc ) _area::reduce( (const _rect&)*rc ); return 0; }
//! clear
int _lua_area::clearRects(lua_State* L){ _area::clearRects(); return 0; }
//! toRelative
int _lua_area::toRelative(lua_State* L){ _area::toRelative( luaL_checkint( L  , 1 ) , luaL_checkint( L  , 1 ) ); return 0; }
//! clipToIntersect
int _lua_area::clipToIntersect(lua_State* L){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( !rc ) return 0; _area::clipToIntersect( *rc ); return 0; }

//! Lua-_area
const char _lua_area::className[] = "_area";
Lunar<_lua_area>::RegType _lua_area::methods[] = {
  LUNAR_DECLARE_METHOD(_lua_area, add),
  LUNAR_DECLARE_METHOD(_lua_area, toRelative),
  LUNAR_DECLARE_METHOD(_lua_area, clearRects),
  LUNAR_DECLARE_METHOD(_lua_area, clipToIntersect),
  {0,0}
};