#include "_lua/lua.class.area.h"
#include "_lua/lua.class.rect.h"
#include "_lua/lua.func.h"
using namespace _luafunc;

/*##################################
##            Lua-Area            ##
##################################*/

_lua_area::_lua_area( _area a ) : _area( a )
{ }

//! Constructor
_lua_area::_lua_area( lua_State* L ) : _area()
{ }

//! Push-back
int _lua_area::add( lua_State* L ){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( rc ) _area::add( (const _rect&)*rc ); return 0; }

//! Push-back
int _lua_area::reduce( lua_State* L ){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( rc ) _area::reduce( (const _rect&)*rc ); return 0; }

//! clear
int _lua_area::clearRects(lua_State* L){ _area::clearRects(); return 0; }

//! toRelative
int _lua_area::toRelative(lua_State* L){ _area::toRelative( check<int>( L  , 1 ) , check<int>( L  , 2 ) ); return 0; }

//! clipToIntersect
int _lua_area::clipToIntersect(lua_State* L){ _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( !rc ) return 0; _area::clipToIntersect( *rc ); return 0; }

//! dump
int _lua_area::dump(lua_State* L){
	for( _rect r : *this )
		printf("Rect: (%d | %d) %dx%d\n",r.x,r.y,r.width,r.height);
	return 0;
}

//! Lua-_area
const char _lua_area::className[] = "Area";
Lunar<_lua_area>::FunctionType _lua_area::methods[] = {
	LUA_CLASS_FUNC(_lua_area, add),
	LUA_CLASS_FUNC(_lua_area, toRelative),
	LUA_CLASS_FUNC(_lua_area, clearRects),
	LUA_CLASS_FUNC(_lua_area, clipToIntersect),
	LUA_CLASS_FUNC(_lua_area, dump),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_area>::PropertyType _lua_area::properties[] = {
	LUA_CLASS_ATTR_END
};