#include "_lua/lua.class.direntry.h"
#include "_lua/lua.class.bitmap.h"

/*##################################
##            Lua-File            ##
##################################*/

//! Ctor
_lua_direntry::_lua_direntry( _direntry& f ) :
	_direntry( f )
{ }

//! Lua-Ctor
_lua_direntry::_lua_direntry( lua_State* L ) : 
	_direntry( string( luaL_checkstring( L , 1 ) ) )
{ }

int _lua_direntry::open( lua_State* L ){
	lua_pushboolean( L , _direntry::open( luaL_checkstring( L , 1 ) ) );
	return 1;
}

int _lua_direntry::openwrite( lua_State* L ){
	lua_pushboolean( L , _direntry::openwrite() );
	return 1;
}

int _lua_direntry::openread( lua_State* L ){
	lua_pushboolean( L , _direntry::openread() );
	return 1;
}

int _lua_direntry::create( lua_State* L ){
	lua_pushboolean( L , _direntry::create() );
	return 1;
}

int _lua_direntry::exists( lua_State* L ){
	lua_pushboolean( L , _direntry::isExisting() );
	return 1;
}

int _lua_direntry::close( lua_State* L ){
	lua_pushboolean( L , _direntry::close() );
	return 1;
}

int _lua_direntry::readString( lua_State* L ){
	lua_pushstring( L , _direntry::readString( luaL_optint( L , 1 , -1 ) ).c_str() );
	return 1;
}

int _lua_direntry::readChild( lua_State* L )
{
	string child;
	if( !_direntry::readChild( child ) )
		return 0;
	lua_pushstring( L , child.c_str() );
	return 1;
}

int _lua_direntry::rewindChildren( lua_State* L )
{
	_direntry::rewindChildren();
	return 1;
}

int _lua_direntry::writeString( lua_State* L ){
	lua_pushboolean( L , _direntry::writeString( luaL_checkstring( L , 1 ) ) );
	return 1;
}

int _lua_direntry::setAttrs( lua_State* L ){
	lua_pushboolean( L , _direntry::setAttrs( luaL_checkint( L , 1 ) ) );
	return 0;
}

int _lua_direntry::getAttrs( lua_State* L ){
	lua_pushnumber( L , _direntry::getAttrs() );
	return 1;
}

int _lua_direntry::getFileName( lua_State* L ){
	lua_pushstring( L , _direntry::getFileName().c_str() );
	return 1;
}

int _lua_direntry::getName( lua_State* L ){
	lua_pushstring( L , _direntry::getName().c_str() );
	return 1;
}

int _lua_direntry::rename( lua_State* L ){
	lua_pushboolean( L , _direntry::rename( luaL_checkstring( L , 1 ) ) );
	return 1;
}

int _lua_direntry::unlink( lua_State* L ){
	lua_pushboolean( L , _direntry::unlink( luaL_optboolean( L , 1 , false ) ) );
	return 1;
}

int _lua_direntry::isDirectory( lua_State* L ){
	lua_pushboolean( L , _direntry::isDirectory() );
	return 1;
}

int _lua_direntry::getExtension( lua_State* L ){
	lua_pushstring( L , _direntry::getExtension().c_str() );
	return 1;
}

int _lua_direntry::getMimeType( lua_State* L ){
	lua_pushstring( L , string(_direntry::getMimeType()).c_str() );
	return 1;
}

int _lua_direntry::getSize( lua_State* L ){
	lua_pushnumber( L , _direntry::getSize() );
	return 1;
}

int _lua_direntry::execute( lua_State* L ){
	lua_pushboolean( L , _direntry::execute() );
	return 1;
}

int _lua_direntry::getFileImage( lua_State* L ){
	Lunar<_lua_bitmap>::push( L , new _lua_bitmap( const_cast<_bitmap&&>( _direntry::getFileImage() ) ) );
	return 1;
}

//! Lua-_gadget
const char _lua_direntry::className[] = "_direntry";
Lunar<_lua_direntry>::FunctionType _lua_direntry::methods[] = {
	LUA_CLASS_FUNC(_lua_direntry,open),
	LUA_CLASS_FUNC(_lua_direntry,openwrite),
	LUA_CLASS_FUNC(_lua_direntry,openread),
	LUA_CLASS_FUNC(_lua_direntry,create),
	LUA_CLASS_FUNC(_lua_direntry,exists),
	LUA_CLASS_FUNC(_lua_direntry,close),
	LUA_CLASS_FUNC(_lua_direntry,readChild),
	LUA_CLASS_FUNC(_lua_direntry,rewindChildren),
	LUA_CLASS_FUNC(_lua_direntry,readString),
	LUA_CLASS_FUNC(_lua_direntry,writeString),
	LUA_CLASS_FUNC(_lua_direntry,getFileName),
	LUA_CLASS_FUNC(_lua_direntry,getName),
	LUA_CLASS_FUNC(_lua_direntry,getExtension),
	LUA_CLASS_FUNC(_lua_direntry,getMimeType),
	LUA_CLASS_FUNC(_lua_direntry,execute),
	LUA_CLASS_FUNC(_lua_direntry,rename),
	LUA_CLASS_FUNC(_lua_direntry,unlink),
	LUA_CLASS_FUNC(_lua_direntry,getFileImage),
	LUA_CLASS_FUNC(_lua_direntry,isDirectory),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_direntry>::PropertyType _lua_direntry::properties[] = {
	{ "attributes" , &_lua_direntry::getAttrs , &_lua_direntry::setAttrs },
	{ "size" , &_lua_direntry::getSize , nullptr },
	LUA_CLASS_ATTR_END
};