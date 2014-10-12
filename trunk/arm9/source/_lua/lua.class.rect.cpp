#include <_lua/lua.class.rect.h>
#include <_lua/lua.class.area.h>
#include <_lua/lua.func.h>
using namespace _luafunc;

/*##################################
##            Lua-Rect            ##
##################################*/

_lua_rect::_lua_rect( _rect rc ) : _rect( rc )
{}

//! Constructor
_lua_rect::_lua_rect( lua_State* L ) : 
	_rect( check<_coord>( L , 1 ) , check<_coord>( L , 2 ) , check<_length>( L , 3 ) , check<_length>( L , 4 ) )
{ }
//! Set X
int _lua_rect::setX(lua_State* L){ _rect::x = check<_coord>( L , 1 ); return 0; }
//! Set Y
int _lua_rect::setY(lua_State* L){ _rect::y = check<_coord>( L , 1 ); return 0; }
//! Get X
int _lua_rect::getX(lua_State* L){ push( L , _rect::x ); return 1; }
//! Get Y
int _lua_rect::getY(lua_State* L){ push( L , _rect::y ); return 1; }
//! Set X2
int _lua_rect::setX2(lua_State* L){ _rect::setX2( check<_coord>( L , 1 ) ); return 0; }
//! Set Y2
int _lua_rect::setY2(lua_State* L){ _rect::setY2( check<_coord>( L , 1 ) ); return 0; }
//! Get X2
int _lua_rect::getX2(lua_State* L){ push( L , _rect::getX2() ); return 1; }
//! Get Y2
int _lua_rect::getY2(lua_State* L){ push( L , _rect::getY2() ); return 1; }
//! Set Width
int _lua_rect::setWidth(lua_State* L){ _rect::width = check<_length>( L , 1 ); return 0; }
//! Set Height
int _lua_rect::setHeight(lua_State* L){ _rect::height = check<_length>( L , 1 ); return 0; }
//! Get Width
int _lua_rect::getWidth(lua_State* L){ push( L , _rect::width ); return 1; }
//! Get Height
int _lua_rect::getHeight(lua_State* L){ push( L , _rect::height ); return 1; }
//! isValid
int _lua_rect::isValid(lua_State* L){ push( L , _rect::isValid() ); return 1; }
//! Contains (Coords)
int _lua_rect::contains(lua_State* L){
	if( is_a<_rect>( L , 1 ) )
		return push( L , _rect::contains( check<_rect>( L , 1 ) ) );
	return push( L , _rect::contains( check<_coord>( L , 1 ) , check<_coord>( L , 2 ) ) );
}
//! fromCoords
int _lua_rect::fromCoords(lua_State* L){
	return push( L ,
		_rect::fromCoords( 
			check<_coord>( L , 1 ),
			check<_coord>( L , 2 ),
			check<_coord>( L , 3 ),
			check<_coord>( L , 4 )
		)
	);
}
//! toRelative
int _lua_rect::toRelative(lua_State* L){ _rect::toRelative( check<int>( L , 1 ) , check<int>( L , 2 ) ); return 0; }
//! AND
int _lua_rect::clipToIntersect(lua_State* L){ _rect::clipToIntersect( check<_rect>( L , 1 ) ); return 0; }
//! +
int _lua_rect::expandToInclude(lua_State* L){ _rect::expandToInclude( check<_rect>( L , 1 ) ); return 0; }
//! -
int _lua_rect::reduce(lua_State* L){ return push( L , _rect::reduce( check<_rect>( L , 1 ) ) ); }
//! OR
int _lua_rect::combine(lua_State* L){ return push( L , _rect::combine( check<_rect>( L , 1 ) ) ); }
//! intersectsWith
int _lua_rect::intersectsWith(lua_State* L){ return push( L , _rect::intersectsWith( check<_rect>( L , 1 ) ) ); }

//! Lua-_rect
const char _lua_rect::className[] = "Rect";
Lunar<_lua_rect>::FunctionType _lua_rect::methods[] = {
  LUA_CLASS_FUNC(_lua_rect, setX2),
  LUA_CLASS_FUNC(_lua_rect, setY2),
  LUA_CLASS_FUNC(_lua_rect, getX2),
  LUA_CLASS_FUNC(_lua_rect, getY2),
  LUA_CLASS_FUNC(_lua_rect, isValid),
  LUA_CLASS_FUNC(_lua_rect, contains),
  LUA_CLASS_FUNC(_lua_rect, toRelative),
  LUA_CLASS_FUNC(_lua_rect, clipToIntersect),
  LUA_CLASS_FUNC(_lua_rect, expandToInclude),
  LUA_CLASS_FUNC(_lua_rect, reduce),
  LUA_CLASS_FUNC(_lua_rect, combine),
  LUA_CLASS_FUNC(_lua_rect, intersectsWith),
  LUA_CLASS_FUNC_END
};

Lunar<_lua_rect>::StaticType _lua_rect::staticmethods[] = {
	{ "fromCoords" , &_lua_rect::fromCoords }
};

Lunar<_lua_rect>::PropertyType _lua_rect::properties[] = {
	{ "x" , &_lua_rect::getX , &_lua_rect::setX },
	{ "y" , &_lua_rect::getY , &_lua_rect::setY },
	{ "width" , &_lua_rect::getWidth , &_lua_rect::setWidth },
	{ "height" , &_lua_rect::getHeight , &_lua_rect::setHeight },
	LUA_CLASS_ATTR_END
};