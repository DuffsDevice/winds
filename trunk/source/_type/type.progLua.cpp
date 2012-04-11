#include "_type/type.progLua.h"
#include "_type/type.system.h"

#include "_lua/lunar.h"
/**
 * Proxy Classes
**/
#include "_lua/lua.class.rect.h"
#include "_lua/lua.class.font.h"
#include "_lua/lua.class.file.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.class.bitmapPort.h"
#include "_lua/lua.class.gadgetEventArgs.h"
#include "_lua/lua.class.window.h"
#include "_lua/lua.class.label.h"
#include "_lua/lua.class.imagegadget.h"
#include "_lua/lua.class.button.h"
#include "_lua/lua.class.checkbox.h"

int luaL_expectint(lua_State* L , int narg , string name ){
	if( !lua_isnumber(L,narg) ){
		printf("Arg. #%d of %s is bad: %s\n",narg,name.c_str(),luaL_typename(L,narg));
		return 0;
	}
	return lua_tonumber( L , narg );
}

string luaL_expectstring(lua_State* L , int narg , string name ){
	if( !lua_isstring(L,narg) ){
		printf("Arg. #%d of %s is bad: %s\n",narg,name.c_str(),luaL_typename(L,narg));
		return "";
	}
	return lua_tostring( L , narg );
}

bool luaL_is( lua_State* L , int narg , string type ){
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

_windows* cur = NULL;

int addToWindows( lua_State* L ){ _lua_gadget* g = Lunar<_lua_window>::check( L , 1 ); if( g && cur ) cur->addChild( g->gadget ); return 0; }
int luaRGB( lua_State* L ){ lua_pushnumber( L , RGB( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) ) ); return 1; }
int luaRGBA( lua_State* L ){ lua_pushnumber( L , RGBA( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) ) ); return 1; }


//! This Function will be called from _gadget
_gadgetEventReturnType lua_callEventHandler( lua_State* L , int handler , _gadgetEvent e ){
	//! No state registered?
	if( !L )
		return not_handled;
	//! Put the Lua-Handler-Function on top of the Stack
	lua_rawgeti( L , LUA_REGISTRYINDEX, handler );
	
	//! Call it with a param
	Lunar<_lua_gadgetEvent>::push( L , new _lua_gadgetEvent( e ) , true );
	lua_call( L , 1 , 1 ); // 1 parameter and 1 return value
	
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
	Lunar<_lua_font>::Register( this->state );
	Lunar<_lua_file>::Register( this->state );
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
	Lunar<_lua_checkbox>::Register( this->state );
	Lunar<_lua_label>::Register( this->state );
	
	// Load our lua-piece
	luaL_loadstring( this->state , this->code.c_str() );
	
	// Publish some methods
	lua_pushcfunction( this->state , addToWindows );
	lua_setglobal( this->state , "registerToWindows" );
	
	lua_pushcfunction( this->state , luaRGB );
	lua_setglobal( this->state , "RGB" );
	
	lua_pushcfunction( this->state , luaRGBA );
	lua_setglobal( this->state , "RGBA" );
	
	// Parse Whole Program
	if( lua_pcall( this->state , 0 , 0 , 0 ) ){
		_system_->debug( string( "Lua-Parser-Error: " ) + lua_tostring( this->state , -1 ) );
	}
}

bool _progLua::runInit(){
	lua_getglobal( this->state , "init" );
	if( lua_isfunction( this->state , -1 ) && lua_pcall( this->state , 0 , LUA_MULTRET , 0 ) ){
		_system_->debug( string( "Lua-Error in init(): " ) + lua_tostring( this->state , -1 ) );
		return false;
	}
	return true;
}

void _progLua::runMain(){
	lua_getglobal( this->state , "main" );
	if( lua_isfunction( this->state , -1 ) && lua_pcall( this->state , 0 , LUA_MULTRET , 0 ) ){
		_system_->debug( string( "Lua-Error in main(): " ) + lua_tostring( this->state , -1 ) );
	}
}

void _progLua::run(){	
	if( this->runInit() )
		this->runMain();
}