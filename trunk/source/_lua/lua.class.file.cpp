#include "_lua/lua.class.file.h"
#include "_lua/lua.class.bitmap.h"

/*##################################
##            Lua-File            ##
##################################*/

//! Ctor
_lua_file::_lua_file( _file* f ) :
	file( f )
{ }

//! Lua-Ctor
_lua_file::_lua_file( lua_State* L ) : 
	file( new _file( string( luaL_checkstring( L , 1 ) ) ) )
{ }

_lua_file::~_lua_file(){
	if( this->file )
		delete this->file;
}

int _lua_file::open( lua_State* L ){
	lua_pushboolean( L , this->file->open( luaL_checkstring( L , 1 ) ) );
	return 1;
}

int _lua_file::openwrite( lua_State* L ){
	lua_pushboolean( L , this->file->openwrite() );
	return 1;
}

int _lua_file::openread( lua_State* L ){
	lua_pushboolean( L , this->file->openread() );
	return 1;
}

int _lua_file::create( lua_State* L ){
	lua_pushboolean( L , this->file->create( luaL_optint( L , 1 , false ) ) );
	return 1;
}

int _lua_file::exists( lua_State* L ){
	lua_pushboolean( L , this->file->exists() );
	return 1;
}

int _lua_file::close( lua_State* L ){
	lua_pushboolean( L , this->file->close() );
	return 1;
}

int _lua_file::readString( lua_State* L ){
	lua_pushstring( L , this->file->readString( luaL_optint( L , 1 , -1 ) ).c_str() );
	return 1;
}

int _lua_file::writeString( lua_State* L ){
	lua_pushboolean( L , this->file->writeString( luaL_checkstring( L , 1 ) ) );
	return 1;
}

int _lua_file::setAttrs( lua_State* L ){
	lua_pushboolean( L , this->file->setAttrs( luaL_checkint( L , 1 ) ) );
	return 1;
}

int _lua_file::getFileName( lua_State* L ){
	lua_pushstring( L , this->file->getFileName().c_str() );
	return 1;
}

int _lua_file::getName( lua_State* L ){
	lua_pushstring( L , this->file->getName().c_str() );
	return 1;
}

int _lua_file::getExtension( lua_State* L ){
	lua_pushstring( L , this->file->getExtension().c_str() );
	return 1;
}

int _lua_file::getMimeTypeString( lua_State* L ){
	lua_pushstring( L , string(this->file->getMimeType()).c_str() );
	return 1;
}

int _lua_file::getSize( lua_State* L ){
	lua_pushnumber( L , this->file->getSize() );
	return 1;
}

int _lua_file::execute( lua_State* L ){
	lua_pushboolean( L , this->file->execute() );
	return 1;
}

int _lua_file::getFileImage( lua_State* L ){
	Lunar<_lua_bitmap>::push( L , new _lua_bitmap( const_cast<_bitmap*>( this->file->getFileImage() ) ) , true );
	return 1;
}

//! Lua-_gadget
const char _lua_file::className[] = "_file";
Lunar<_lua_file>::RegType _lua_file::methods[] = {
	LUNAR_DECLARE_METHOD(_lua_file,open),
	LUNAR_DECLARE_METHOD(_lua_file,openwrite),
	LUNAR_DECLARE_METHOD(_lua_file,openread),
	LUNAR_DECLARE_METHOD(_lua_file,create),
	LUNAR_DECLARE_METHOD(_lua_file,exists),
	LUNAR_DECLARE_METHOD(_lua_file,close),
	LUNAR_DECLARE_METHOD(_lua_file,readString),
	LUNAR_DECLARE_METHOD(_lua_file,writeString),
	LUNAR_DECLARE_METHOD(_lua_file,setAttrs),
	LUNAR_DECLARE_METHOD(_lua_file,getFileName),
	LUNAR_DECLARE_METHOD(_lua_file,getName),
	LUNAR_DECLARE_METHOD(_lua_file,getExtension),
	LUNAR_DECLARE_METHOD(_lua_file,getMimeTypeString),
	LUNAR_DECLARE_METHOD(_lua_file,getSize),
	LUNAR_DECLARE_METHOD(_lua_file,execute),
	LUNAR_DECLARE_METHOD(_lua_file,getFileImage),
	{0,0}
};