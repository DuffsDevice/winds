#include "_type/type.luainstance.h"

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
_program::_program( string prog ) : 
	code( prog )
{
	// Create State
	this->state = luaL_newstate();
	
	// Open standard functions like math, table-functions etc...
	luaL_openlibs( this->state );
	
	// Register Class _window
	//Lunar<_lua_window>::Register( this->state );
	Lunar<_lua_rect>::Register( this->state );
	Lunar<_lua_area>::Register( this->state );
	Lunar<_lua_bitmap>::Register( this->state );
	Lunar<_lua_gadgetEventArgs>::Register( this->state );
	Lunar<_lua_gadgetEvent>::Register( this->state );
	Lunar<_lua_gadget>::Register( this->state );
	Lunar<_lua_window>::Register( this->state );
	Lunar<_lua_button>::Register( this->state );
	Lunar<_lua_checkbox>::Register( this->state );
}

void _program::runInit(){
	lua_getglobal( this->state , "init" );
	if( lua_isfunction( this->state , -1 ) && lua_pcall( this->state , 0 , LUA_MULTRET , 0 ) ){
		printf("Epic Fail Init!");
		while(true);
	}
}

void _program::runMain(){
	lua_getglobal( this->state , "main" );
	if( lua_isfunction( this->state , -1 ) && lua_pcall( this->state , 0 , LUA_MULTRET , 0 ) ){
		printf("Epic Fail Main!");
		while(true);
	}
}

_windows* cur = NULL;

int addToWindows( lua_State* L ){ _lua_gadget* g = Lunar<_lua_window>::check( L , 1 ); if( g && cur ) cur->addChild( g->gadget ); return 0; }

void _program::run( _windows* w ){
	srand( time(NULL) );
	cur = w;
	luaL_loadstring( this->state , this->code.c_str() );
	lua_pushcfunction( this->state , addToWindows );
	lua_setglobal( this->state , "registerToWindows" );
	if( lua_pcall( this->state , 0 , 0 , 0 ) ){
		printf("Parser Error");
	}
	this->runInit();
	this->runMain();
}