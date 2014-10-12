#ifndef _WIN_L_FUNC_
	#include <_lua/lua.func.h>
#endif

#ifndef _WIN_L_CALLBACK_2_
#define _WIN_L_CALLBACK_2_

#include <_controller/controller.debug.h>

// ~~~~~~~~~~~~~~~~~~~~~~~ A Lua-Callback ~~~~~~~~~~~~~~~~~~~~~~~
template<typename T, typename... Parameters>
T _luaCallback<T(Parameters...)>::operator()(Parameters... params) const
{
	// Is the function valid?
	if( !state || index == LUA_NOREF )
		return T();
	
	// Put the Lua-Handler-Function on top of the Stack
	lua_rawgeti( state , LUA_REGISTRYINDEX, index );
	
	// Push the parameters
	_luafunc::push( state , forward<Parameters>(params)... );
	
	// Call the callback
	if( lua_pcall( state , sizeof...(Parameters) , 1 , 0 ) )
		_debugController::luaErrorHandler( state , lua_tostring( state , -1 ) );
	
	if( std::is_same<T,void>::value || lua_isnil( state , -1 ) )
		return T();
	
	// Forward the Value returned by the Handler
	return _luafunc::check<T>( state , -1 );
}

#endif