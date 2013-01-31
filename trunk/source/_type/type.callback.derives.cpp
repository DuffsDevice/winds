#include "_type/type.callback.derives.h"
#include "_type/type.system.h"
#include "_lua/lunar.h"
#include "_lua/lua.class.event.h"

//
// STATIC
//

void _staticCallback::operator()() const {
	if( this->funcType == _callbackType::voidFunc && this->voidFn )
		this->voidFn();
}

int _staticCallback::operator()( int i ) const {
	if( this->intFn )
	{
		if( this->funcType == _callbackType::intFunc)
			return this->intFn( i );
		
		// Allow calling an event Handler which is actually a void function
		else if( this->funcType == _callbackType::voidFunc )
			this->voidFn();
		return 0;
	}
	return -1;
}

_callbackReturn _staticCallback::operator()( _event e ) const {
	if( this->eventFn )
	{
		if( this->funcType == _callbackType::eventFunc )
			return this->eventFn( e );
		
		// Allow calling an event Handler which is actually a void function
		else if( this->funcType == _callbackType::voidFunc )
			this->voidFn();
		
		return handled;
	}
	
	return not_handled;
}


//
// CLASS
//

void _classCallback::operator()() const {
	if( this->voidFn && this->instance )
		(instance->*voidFn)();
}

int _classCallback::operator()( int i ) const {
	if( this->intFn && this->instance )
	{
		if( this->funcType == _callbackType::intFunc )
			return (this->instance->*intFn)( i );
		
		// Allow calling an event Handler which is actually a void function
		else if( this->funcType == _callbackType::voidFunc )
			(this->instance->*voidFn)();
		
		return 0;
	}
	
	return -1;
}

_callbackReturn _classCallback::operator()( _event e ) const {
	if( this->eventFn && this->instance )
	{
		if( this->funcType == _callbackType::eventFunc )
			return (this->instance->*eventFn)( e );
		
		// Allow calling an event Handler which is actually a void function
		else if( this->funcType == _callbackType::voidFunc )
			(this->instance->*voidFn)();
		
		return handled;
	}
	
	return not_handled;
}

//
// INLINE
//

void _inlineCallback::operator()() const {
	if( this->voidFn )
		this->voidFn();
}

int _inlineCallback::operator()( int i ) const {
	if( this->intFn )
		return this->intFn( i );
		
	// Allow calling an event Handler which is actually a void function
	if( this->voidFn )
	{
		this->voidFn();
		return 0;
	}
	
	return -1;
}

_callbackReturn _inlineCallback::operator()( _event e ) const {
	if( this->eventFn )
		return this->eventFn( e );
		
	// Allow calling an event Handler which is actually a void function
	else if( this->voidFn )
	{
		this->voidFn();
		return handled;
	}
	
	return not_handled;
}

//
// LUA
//

void _luaCallback::operator()() const {
	lua_callVoidFn( state , index );
}

int _luaCallback::operator()( int i ) const {
	return lua_callIntFn( state , index , i );
}

_callbackReturn _luaCallback::operator()( _event e ) const {
	return lua_callEventFn( state , index , e );
}

_u8 _luaCallback::equals( const _callback& param ) const 
{
	if( ((_luaCallback&)param).state != this->state )
		return 0;
	if( ((_luaCallback&)param).index == LUA_NOREF || this->index == LUA_NOREF )
		return -1;
	if( ((_luaCallback&)param).index == this->index )
		return 1;
	
	// Push both inices onto stack and compare!
	lua_rawgeti( this->state , LUA_REGISTRYINDEX, index );
	lua_rawgeti( this->state , LUA_REGISTRYINDEX, ((_luaCallback&)param).index );
	
	if ( lua_rawequal(this->state, -1, -2) ) // Are they equal?
	{ 
		lua_pop(this->state, 2); // Remove both values
		return 1;
	}
	lua_pop(this->state, 2); // Remove both values
	return -1;
}

_luaCallback::~_luaCallback()
{
	if( this->state )
		lua_popFunction( state , index );
}


//! This Function will be called from _gadget
_callbackReturn lua_callEventFn( lua_State* L , int handler , _event e )
{
	//! No state registered?
	if( !L || handler == LUA_NOREF )
		return not_handled;
	
	//! Put the Lua-Handler-Function on top of the Stack
	lua_rawgeti( L , LUA_REGISTRYINDEX, handler );
	
	//! Call it with a param
	Lunar<_lua_event>::push( L , new _lua_event( e ) );
	
	if( lua_pcall( L , 1 , 1 , 0 ) ){
		_system::debug( string( "Callback-Lua-Err: " ) + lua_tostring( L , -1 ) );
	}
	
	//! Return the Value returned by the Handler
	return string2eventReturnType[ luaL_optstring( L , -1 , "not_handled" ) ];
}

//! This Function will be called from _gadget
int lua_callIntFn( lua_State* L , int handler , int i )
{
	//! No state registered?
	if( !L || handler == LUA_NOREF )
		return -1;
	
	//! Put the Lua-Handler-Function on top of the Stack
	lua_rawgeti( L , LUA_REGISTRYINDEX, handler );
	
	//! Call it with one param
	lua_pushnumber( L , i );
	
	if( lua_pcall( L , 1 , 1 , 0 ) ){
		_system::debug( string( "Callback-Lua-Err: " ) + lua_tostring( L , -1 ) );
	}
	
	//! Return the Value returned by the Handler
	return luaL_optint( L , -1 , 0 );
}

//! This Function will be called from _gadget
void lua_callVoidFn( lua_State* L , int handler )
{
	//! No state registered?
	if( !L || handler == LUA_NOREF )
		return;
	
	//! Put the Lua-Handler-Function on top of the Stack
	lua_rawgeti( L , LUA_REGISTRYINDEX, handler );
	
	//! Call it
	if( lua_pcall( L , 0 , 0 , 0 ) ){
		_system::debug( string( "Callback-Lua-Err: " ) + lua_tostring( L , -1 ) );
	}
}

int lua_checkFunction( lua_State* L , int narg )
{
	if( L && lua_isfunction( L , narg ) )
	{
		lua_pushvalue( L , narg ); // Copy
		int ret = luaL_ref( L , LUA_REGISTRYINDEX );
		return ret;
	}
	
	return LUA_NOREF;
}

void lua_popFunction( lua_State* L , int index )
{
	if( L )
		luaL_unref( L , LUA_REGISTRYINDEX , index );
}