#include <_lua/lua.class.mime.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*#############################
##           Lua-Ini         ##
#############################*/

_lua_mimetype::_lua_mimetype( lua_State* L ) :
	_mimeType( check<string>( L , 1 ) )
{}

_lua_mimetype::_lua_mimetype( const _mimeType& mT ) :
	_mimeType( mT )
{}

//! equals
int _lua_mimetype::equals( lua_State* L ){
	return push( L , check<_mimeType>( L , 1 ) == *(_mimeType*)this );
}

//! fromExtension
int _lua_mimetype::fromExtension( lua_State* L ){
	return push( L , _mimeType::fromExtension( check<string>( L , 1 ) ) );
}


//! Lua-_mimeType
const char _lua_mimetype::className[] = "MimeType";
Lunar<_lua_mimetype>::FunctionType _lua_mimetype::methods[] = {
	{ "__tostring"		, wrap( _lua_mimetype , &_mimeType::operator _literal ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_mimetype>::PropertyType _lua_mimetype::properties[] = {
	LUA_CLASS_ATTR_END
};

Lunar<_lua_mimetype>::StaticType	_lua_mimetype::staticmethods[] = {
	{ "fromExtension"	, &_lua_mimetype::fromExtension },
	LUA_CLASS_FUNC_END
};