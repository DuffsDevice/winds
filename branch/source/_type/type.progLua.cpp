#include "_type/type.progLua.h"
#include "_type/type.system.h"

#include "_lua/lunar.h"
/**
 * Proxy Classes
**/
#include "_lua/lua.class.rect.h"
#include "_lua/lua.class.border.h"
//#include "_lua/lua.class.font.h"
#include "_lua/lua.class.direntry.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.class.bitmapPort.h"
#include "_lua/lua.class.gadgetEventArgs.h"
#include "_lua/lua.class.window.h"
#include "_lua/lua.class.label.h"
#include "_lua/lua.class.imagegadget.h"
#include "_lua/lua.class.button.h"
#include "_lua/lua.class.select.h"
#include "_lua/lua.class.checkbox.h"

int luaL_expectint(lua_State* L , int narg , string name )
{
	if( !lua_isnumber(L,narg) )
	{
		printf("Arg. #%d of %s is bad: %s\n",narg,name.c_str(),luaL_typename(L,narg));
		lua_pop( L , 1 );
		return 0;
	}
	int num = lua_tonumber( L , narg );
	lua_pop( L , 1 );
	return num;
}

string luaL_expectstring(lua_State* L , int narg , string name )
{
	if( !lua_isstring(L,narg) ){
		printf("Arg. #%d of %s is bad: %s\n",narg,name.c_str(),luaL_typename(L,narg));
		lua_pop( L , 1 );
		return "";
	}
	string str = lua_tostring( L , narg );
	lua_pop( L , 1 );
	return str;
}

bool luaL_is( lua_State* L , int narg , string type )
{
	void *p = lua_touserdata(L, narg);
	if (p != NULL) {  /* value is a userdata? */
		if (lua_getmetatable(L, narg)) {  /* does it have a metatable? */
			lua_getfield(L, LUA_REGISTRYINDEX, type.c_str());  /* get correct metatable */
			if (lua_rawequal(L, -1, -2)) {  /* does it have the correct mt? */
				lua_pop(L, 2);  /* remove both metatables */
				return true;
			}
		}
	}
	return false;
}

int registerWindow( lua_State* L ){ _lua_gadget* g = Lunar<_lua_window>::check( L , 1 ); if( !g ) return 0; _system_->_gadgetHost_->addChild( g->gadget ); return 0; }

int readRegistryIndex( lua_State* L ){ lua_pushstring( L , _system_->_registry_->readIndex( luaL_checkstring( L , 1 ) , luaL_checkstring( L , 2 ) ).c_str() ); return 1; }
int writeRegistryIndex( lua_State* L ){ _system_->_registry_->writeIndex( luaL_checkstring( L , 1 ) , luaL_checkstring( L , 2 ) , luaL_checkstring( L , 3 ) ); return 0; }
int deleteRegistryIndex( lua_State* L ){ _system_->_registry_->deleteIndex( luaL_checkstring( L , 1 ) , luaL_checkstring( L , 2 ) ); return 0; }
int deleteRegistrySection( lua_State* L ){ _system_->_registry_->deleteSection( luaL_checkstring( L , 1 ) ); return 0; }

int luaRGB( lua_State* L ){ lua_pushnumber( L , RGB( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) ) ); return 1; }
int luaRGBA( lua_State* L ){ lua_pushnumber( L , RGBA( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) ) ); return 1; }
int luaRGB_GETR( lua_State* L ){ lua_pushnumber( L , RGB_GETR( luaL_checkint( L , 1 ) ) ); return 1; }
int luaRGB_GETG( lua_State* L ){ lua_pushnumber( L , RGB_GETG( luaL_checkint( L , 1 ) ) ); return 1; }
int luaRGB_GETB( lua_State* L ){ lua_pushnumber( L , RGB_GETB( luaL_checkint( L , 1 ) ) ); return 1; }
int luaRGB_GETA( lua_State* L ){ lua_pushnumber( L , RGB_GETA( luaL_checkint( L , 1 ) ) ); return 1; }

luaL_Reg windowsLibrary[] = {
	{"RGB",luaRGB},
	{"RGBA",luaRGBA},
	{"RGB_GETR",luaRGB_GETR},
	{"RGB_GETG",luaRGB_GETG},
	{"RGB_GETB",luaRGB_GETB},
	{"RGB_GETA",luaRGB_GETA},
	{"registerWindow",registerWindow},
	{"readRegistryIndex",readRegistryIndex},
	{"writeRegistryIndex",writeRegistryIndex},
	{"deleteRegistryIndex",deleteRegistryIndex},
	{"deleteRegistrySection",deleteRegistrySection},
	{ 0 , 0 }
};


//! This Function will be called from _gadget
_gadgetEventReturnType lua_callEventHandler( lua_State* L , int handler , _gadgetEvent e )
{
	//! No state registered?
	if( !L )
		return not_handled;
	
	//! Put the Lua-Handler-Function on top of the Stack
	lua_rawgeti( L , LUA_REGISTRYINDEX, handler );
	
	//! Call it with a param
	Lunar<_lua_gadgetEvent>::push( L , new _lua_gadgetEvent( e ) , true );
	lua_pcall( L , 1 , 1 , 0 ); // 1 parameter and 1 return value
	
	//! Return the Value returned by the Handler
	return string2eventReturnType[ luaL_expectstring( L , -1 , "_gadget::eventHandler" ) ];
}


/**
 * Programm Stuff
**/
_progLua::_progLua( string prog ) : 
	_program( progLua )
	, code( prog )
{
	// Create State
	this->state = luaL_newstate();
	
	// Open standard functions like math, table-functions etc...
	luaL_openlibs( this->state );
	
	//! Register Base Classes
	Lunar<_lua_rect>::Register( this->state );
	Lunar<_lua_border>::Register( this->state );
	//Lunar<_lua_font>::Register( this->state );
	Lunar<_lua_direntry>::Register( this->state );
	Lunar<_lua_area>::Register( this->state );
	Lunar<_lua_bitmap>::Register( this->state );
	Lunar<_lua_bitmapPort>::Register( this->state );
	Lunar<_lua_gadgetEventArgs>::Register( this->state );
	Lunar<_lua_gadgetEvent>::Register( this->state );
	Lunar<_lua_gadget>::Register( this->state );
	
	//! Register Gadgets
	Lunar<_lua_window>::Register( this->state );
	Lunar<_lua_imagegadget>::Register( this->state );
	Lunar<_lua_button>::Register( this->state );
	Lunar<_lua_select>::Register( this->state );
	Lunar<_lua_checkbox>::Register( this->state );
	Lunar<_lua_label>::Register( this->state );
	
	// Load our lua-piece
	luaL_loadstring( this->state , this->code.c_str() );
	
	
	//! Load System.---
	lua_newtable( this->state );
	int top = lua_gettop( this->state );

	int i = 0;
	while( windowsLibrary[i].name )
	{
		lua_pushstring( this->state , windowsLibrary[i].name );
		lua_pushcfunction( this->state , windowsLibrary[i].func );
		lua_settable( this->state , top );
		i++;
	}
	
	lua_setglobal( this->state , "System" );
	
	
	// Parse Whole Program
	if( lua_pcall( this->state , 0 , 0 , 0 ) ){
		_system_->debug( string( "Lua-Parser-Error: " ) + lua_tostring( this->state , -1 ) );
	}
	
}

void _progLua::init( _cmdArgs& args )
{
	lua_getglobal( this->state , "init" );
	
	if( lua_isfunction( this->state , -1 ) )
	{
		// Create Arguments
		lua_newtable( this->state );
		int top = lua_gettop( this->state );
		
		for( _cmdArgs::iterator it = args.begin() ; it != args.end() ; ++it )
		{
			lua_pushstring( this->state , it->first.c_str() ); // Key
			lua_pushstring( this->state , it->second.c_str() ); // Value
			lua_settable( this->state , top );
		}
		
		if( lua_pcall( this->state , 1 /* One Argument */ , 0 , 0 ) )
			_system_->debug( string( "Lua-Error in init(): " ) + lua_tostring( this->state , -1 ) );
	}
}

int _progLua::main( _cmdArgs& args )
{
	// Get Function
	lua_getglobal( this->state , "main" );
	
	if( lua_isfunction( this->state , -1 ) )
	{
		// Create Arguments
		lua_newtable( this->state );
		int top = lua_gettop( this->state );
		
		for( _cmdArgs::iterator it = args.begin() ; it != args.end() ; ++it )
		{
			lua_pushstring( this->state , it->first.c_str() ); // Key
			lua_pushstring( this->state , it->second.c_str() ); // Value
			lua_settable( this->state , top );
		}
		
		if( lua_pcall( this->state , 1 /* One Argument */ , LUA_MULTRET , 0 ) )
			_system_->debug( string( "Lua-Error in main(): " ) + lua_tostring( this->state , -1 ) );
	}
	
	// Get Return-value
	int t = luaL_optint( this->state , -1 , 0 );
	
	// Reset Lua-Stack
	lua_settop( this->state , 0 );
	
	return t;
}

_progLua::~_progLua()
{
	lua_close( this->state );
}