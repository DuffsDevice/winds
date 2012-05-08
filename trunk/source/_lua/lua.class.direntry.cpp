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
	lua_pushstring( L ,  child.c_str() );
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

int _lua_direntry::getExtension( lua_State* L ){
	lua_pushstring( L , _direntry::getExtension().c_str() );
	return 1;
}

int _lua_direntry::getMimeTypeString( lua_State* L ){
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
	Lunar<_lua_bitmap>::push( L , new _lua_bitmap( const_cast<_bitmap*>( _direntry::getFileImage() ) ) , true );
	return 1;
}

//! Lua-_gadget
const char _lua_direntry::className[] = "_direntry";
Lunar<_lua_direntry>::RegType _lua_direntry::methods[] = {
	LUNAR_DECLARE_METHOD(_lua_direntry,open),
	LUNAR_DECLARE_METHOD(_lua_direntry,openwrite),
	LUNAR_DECLARE_METHOD(_lua_direntry,openread),
	LUNAR_DECLARE_METHOD(_lua_direntry,create),
	LUNAR_DECLARE_METHOD(_lua_direntry,exists),
	LUNAR_DECLARE_METHOD(_lua_direntry,close),
	LUNAR_DECLARE_METHOD(_lua_direntry,readChild),
	LUNAR_DECLARE_METHOD(_lua_direntry,rewindChildren),
	LUNAR_DECLARE_METHOD(_lua_direntry,readString),
	LUNAR_DECLARE_METHOD(_lua_direntry,writeString),
	LUNAR_DECLARE_METHOD(_lua_direntry,setAttrs),
	LUNAR_DECLARE_METHOD(_lua_direntry,getFileName),
	LUNAR_DECLARE_METHOD(_lua_direntry,getName),
	LUNAR_DECLARE_METHOD(_lua_direntry,getExtension),
	LUNAR_DECLARE_METHOD(_lua_direntry,getMimeTypeString),
	LUNAR_DECLARE_METHOD(_lua_direntry,getSize),
	LUNAR_DECLARE_METHOD(_lua_direntry,execute),
	LUNAR_DECLARE_METHOD(_lua_direntry,getFileImage),
	{0,0}
};