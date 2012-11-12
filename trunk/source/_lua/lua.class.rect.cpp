#include "_lua/lua.class.rect.h"
#include "_lua/lua.class.area.h"

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
	Lunar<_lua_rect>::push( L ,
		new _lua_rect( 
			_rect::fromCoords( 
				luaL_checkint( L , 1 ),
				luaL_checkint( L , 2 ),
				luaL_checkint( L , 3 ),
				luaL_checkint( L , 4 )
			)
		) 
	); 
	return 1;
}
//! toRelative
int _lua_rect::toRelative(lua_State* L){ _rect::toRelative( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) ); return 0; }
//! AND
int _lua_rect::clipToIntersect(lua_State* L){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( !rc ) return 0; _rect::clipToIntersect( *rc ); return 0; }
//! +
int _lua_rect::expandToInclude(lua_State* L){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( !rc ) return 0; _rect::expandToInclude( *rc ); return 0; }
//! -
int _lua_rect::reduce(lua_State* L){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( !rc ) return 0; Lunar<_lua_area>::push( L , new _lua_area( _rect::reduce( *rc ) ) ); return 1; }
//! OR
int _lua_rect::combine(lua_State* L){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( !rc ) return 0; Lunar<_lua_area>::push( L , new _lua_area( _rect::combine( *rc ) ) ); return 1; }
//! intersectsWith
int _lua_rect::intersectsWith(lua_State* L){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( !rc ) return 0; lua_pushboolean( L , _rect::intersectsWith( *rc ) ); return 1; }

//! Lua-_rect
const char _lua_rect::className[] = "_rect";
Lunar<_lua_rect>::FunctionType _lua_rect::methods[] = {
  LUA_CLASS_FUNC(_lua_rect, setX),
  LUA_CLASS_FUNC(_lua_rect, setY),
  LUA_CLASS_FUNC(_lua_rect, getX),
  LUA_CLASS_FUNC(_lua_rect, getY),
  LUA_CLASS_FUNC(_lua_rect, setX2),
  LUA_CLASS_FUNC(_lua_rect, setY2),
  LUA_CLASS_FUNC(_lua_rect, getX2),
  LUA_CLASS_FUNC(_lua_rect, getY2),
  LUA_CLASS_FUNC(_lua_rect, setWidth),
  LUA_CLASS_FUNC(_lua_rect, setHeight),
  LUA_CLASS_FUNC(_lua_rect, getWidth),
  LUA_CLASS_FUNC(_lua_rect, getHeight),
  LUA_CLASS_FUNC(_lua_rect, isValid),
  LUA_CLASS_FUNC(_lua_rect, contains),
  LUA_CLASS_FUNC(_lua_rect, fromCoords),
  LUA_CLASS_FUNC(_lua_rect, toRelative),
  LUA_CLASS_FUNC(_lua_rect, clipToIntersect),
  LUA_CLASS_FUNC(_lua_rect, expandToInclude),
  LUA_CLASS_FUNC(_lua_rect, reduce),
  LUA_CLASS_FUNC(_lua_rect, combine),
  LUA_CLASS_FUNC(_lua_rect, intersectsWith),
  LUA_CLASS_FUNC_END
};

Lunar<_lua_rect>::PropertyType _lua_rect::properties[] = {
	LUA_CLASS_ATTR_END
};