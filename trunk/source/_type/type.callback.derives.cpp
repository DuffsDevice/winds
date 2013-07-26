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

int _staticCallback::operator()( int i ) const
{
	if( this->intFn )
	{
		switch( this->funcType )
		{
			case _callbackType::intFunc:
				return this->intFn( i );
				break;
			// Allow calling an event Handler which is actually a void function
			case _callbackType::voidFunc:
				this->voidFn();
				return 1;
			default:
				break;
		}
	}
	return 0;
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
		switch( this->funcType )
		{
			case _callbackType::intFunc:
				return (this->instance->*intFn)( i );
				break;
			// Allow calling an event Handler which is actually a void function
			case _callbackType::voidFunc:
				(this->instance->*voidFn)();
				return 1;
				break;
			default:
				break;
		}
	}
	return 0;
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

_callbackReturn _inlineCallback::operator()( _event&& e ) const {
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


//! This Function will be called from _gadget
_callbackReturn lua_callEventFn( lua_State* L , int handler , _event&& e )
{	
	//! No state registered?
	if( !L || handler == LUA_NOREF )
		return not_handled;
	
	//! Put the Lua-Handler-Function on top of the Stack
	lua_rawgeti( L , LUA_REGISTRYINDEX, handler );
	
	//! Call it with a param
	Lunar<_lua_event>::push( L , new _lua_event( e ) );
	
	if( lua_pcall( L , 1 , 1 , 0 ) ){
		_system::debug( "Callback-Lua-Err: %s" , lua_tostring( L , -1 ) );
	}
	
	//! Return the Value returned by the Handler
	return string2callbackReturn[ luaL_optstring( L , -1 , "not_handled" ) ];
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
		_system::debug( "Callback-Lua-Err: %s" , lua_tostring( L , -1 ) );
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
		_system::debug( "Callback-Lua-Err: %s" , lua_tostring( L , -1 ) );
	}
}

int lua_checkFunction( lua_State* L , int narg )
{
	if( L && lua_isfunction( L , narg ) )
	{
		lua_pushvalue( L , narg ); // Copy
		int ret = luaL_ref( L , LUA_REGISTRYINDEX );
		
		//// Push the handlerCount variabel on top of the stack
		//lua_getfield( L , LUA_REGISTRYINDEX , "__hC__" );
		//
		//int curCount = luaL_optint( L , -1 , 0 );// Get its value
		//
		//curCount++;						// Increase it
		//
		//lua_pop( L , 1 );				// Pop the old value
		//lua_pushnumber( L , curCount );	// Push the new one
		//
		//// write it to the registry again
		//lua_setfield( L , LUA_REGISTRYINDEX , "__hC__" );
		////printf("Register %p has %d\n",L,curCount);
		
		return ret;
	}
	
	return LUA_NOREF;
}

void lua_popFunction( lua_State* L , int index )
{
	if( L && index != LUA_NOREF && index != LUA_REFNIL )
	{
		// Unreference the handler
		luaL_unref( L , LUA_REGISTRYINDEX , index );
		
		//// Push the handlerCount variabel on top of the stack
		//lua_getfield( L , LUA_REGISTRYINDEX , "__hC__" );
		//
		//int curCount = luaL_optint( L , -1 , 0 );// Get its value
		//
		//if( curCount > 0 )						// Decrease it
		//	curCount--;
		//
		//lua_pop( L , 1 );						// Pop the old value
		//lua_pushnumber( L , curCount );			// Push the new one
		//
		//// write it to the registry again
		//lua_setfield( L , LUA_REGISTRYINDEX , "__hC__" );
		////printf("Unreg %p has %d\n",L,curCount);
	}
}