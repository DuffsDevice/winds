#include <_lua/lua.class.direntry.h>
#include <_lua/lua.class.bitmap.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*##################################
##            Lua-File            ##
##################################*/

//! Ctor
_lua_direntry::_lua_direntry( const _direntry& f ) :
	_direntry( f )
{ }

//! Lua-Ctor
_lua_direntry::_lua_direntry( lua_State* L ) : 
	_direntry( check<string>( L , 1 ) )
{ }

int _lua_direntry::readChild( lua_State* L )
{
	_literal child;
	if( is_a<_fileExtensionList>( L , 1 ) )
	{
		_fileExtensionList extensions = check<_fileExtensionList>( L , 1 );
		if( !_direntry::readChild( child , &extensions ) )
			return 0;
	}
	else if( !_direntry::readChild( child ) )
		return 0;
	
	return push( L , child );
}

int _lua_direntry::readChildFolderOnly( lua_State* L )
{
	_literal child;
	if( !_direntry::readChild( child ) )
		return 0;
	
	return push( L , child );
}

int _lua_direntry::getFullName( lua_State* L ){
	return push( L , _direntry::getFullName( lightcheck<bool>( L , 1 , false ) ) );
}

int _lua_direntry::openWrite( lua_State* L ){
	return push( L , _direntry::openWrite( lightcheck<bool>( L , 1 , false ) ) );
}

int _lua_direntry::execute( lua_State* L ){
	return push( L , _direntry::execute( lightcheck<_programArgs>( L , 1 ) ) );
}

//! Lua-_gadget
const char _lua_direntry::className[] = "Direntry";
Lunar<_lua_direntry>::FunctionType _lua_direntry::methods[] = {
	{ "openWrite"			, &_lua_direntry::openWrite },
	{ "openRead"			, wrap( _lua_direntry , &_direntry::openRead ) },
	{ "create"				, wrap( _lua_direntry , &_direntry::create ) },
	{ "exists"				, wrap( _lua_direntry , &_direntry::isExisting ) },
	{ "close"				, wrap( _lua_direntry , &_direntry::close ) },
	{ "readChild"			, &_lua_direntry::readChild },
	{ "readChildFolderOnly"	, &_lua_direntry::readChildFolderOnly },
	{ "rewindChildren"		, wrap( _lua_direntry , &_direntry::rewindChildren ) },
	{ "readString"			, wrap( _lua_direntry , &_direntry::readString ) },
	{ "writeString"			, wrap( _lua_direntry , &_direntry::writeString ) },
	{ "execute"				, &_lua_direntry::execute },
	{ "rename"				, wrap( _lua_direntry , &_direntry::rename ) },
	{ "unlink"				, wrap( _lua_direntry , &_direntry::unlink ) },
	{ "getParentDirectory"	, wrap( _lua_direntry , &_direntry::getParentDirectory ) },
	{ "getFullName"			, &_lua_direntry::getFullName },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_direntry>::PropertyType _lua_direntry::properties[] = {
	{ "fileName"	, wrap( _lua_direntry , &_direntry::getFileName ) , nullptr },
	{ "name"		, wrap( _lua_direntry , &_direntry::getName ) , nullptr },
	{ "extension"	, wrap( _lua_direntry , &_direntry::getExtension ) , nullptr },
	{ "mimeType"	, wrap( _lua_direntry , &_direntry::getMimeType ) , nullptr },
	{ "image"		, wrap( _lua_direntry , &_direntry::getFileImage ) , nullptr },
	{ "size"		, wrap( _lua_direntry , &_direntry::getSize ) , nullptr },
	{ "isDirectory"	, wrap( _lua_direntry , &_direntry::isDirectory ) , nullptr },
	{ "hidden"		, wrap( _lua_direntry , &_direntry::isHidden ) , wrap( _lua_direntry , &_direntry::setHidden ) },
	{ "system"		, wrap( _lua_direntry , &_direntry::isSystem ) , wrap( _lua_direntry , &_direntry::setSystemFile ) },
	{ "readOnly"	, wrap( _lua_direntry , &_direntry::isReadOnly ) , wrap( _lua_direntry , &_direntry::setReadOnly ) },
	LUA_CLASS_ATTR_END
};