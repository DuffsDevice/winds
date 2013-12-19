#ifndef _WIN_L_FUNC_
	#include "_lua/lua.func.h"
#endif

#ifndef _WIN_L_CALLBACK_1_
#define _WIN_L_CALLBACK_1_

#include "_type/type.callback.h"

namespace
{
	static unused int compareCallbacks( lua_State* L1 , lua_State* L2 , int cb1 , int cb2 )
	{
		if( L1 != L2 )									return 0;
		else if( cb1 == LUA_NOREF || cb2 == LUA_NOREF )	return -1;
		else if( cb1 == cb2 )							return 1;
		
		// Push both indices onto stack and compare!
		lua_rawgeti(L1, LUA_REGISTRYINDEX, cb2);
		lua_rawgeti(L1, LUA_REGISTRYINDEX, cb1);
		
		if ( lua_rawequal(L1, -1, -2) ){ // Are they equal? 
			lua_pop(L1, 2); // Remove both values
			return 1;
		}
		
		lua_pop(L1, 2); // Remove both values
		return -1;
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~ A Lua-Callback ~~~~~~~~~~~~~~~~~~~~~~~
template<typename T> class _luaCallback{ };

template<typename T,typename... Parameters>
class _luaCallback<T(Parameters...)> : public _callback<T(Parameters...)>
{
	private:
		
		lua_State* 		state;
		int 			index;
		
		// Internal
		_s8 equals( const _callback<T(Parameters...)>& c ) const 
		{
			const _luaCallback<T(Parameters...)>& cb = (_luaCallback&) c;
			return compareCallbacks( cb.state , this->state , cb.index , this->index );
		}
		
		friend class _progLua;
		
	public:
		
		//! Move ctor needed!
		_luaCallback( _luaCallback&& cb ) :
			_callback<T(Parameters...)>( _callbackType::lua_func )
			, state( cb.state )
			, index( cb.index )
		{
			// invalidate the passed _luaCallback
			cb.state = 0;
		}
		
		//! Ctor
		_luaCallback( lua_State* state , int narg ) :
			_callback<T(Parameters...)>( _callbackType::lua_func )
			, state( state )
		{
			if( state && lua_isfunction( state , narg ) )
			{
				lua_pushvalue( state , narg ); // Copy
				index = luaL_ref( state , LUA_REGISTRYINDEX );
			}
			else
				index = LUA_NOREF;
		}
		
		//! Ctor
		_luaCallback( lua_State* state ) :
			_callback<T(Parameters...)>( _callbackType::lua_func )
			, state( state )
			, index( LUA_NOREF )
		{}
		
		//! Dtor
		~_luaCallback()
		{
			if( state )
				luaL_unref( state , LUA_REGISTRYINDEX , index ); // Unreference the handler
		}
		
		//! Call operator
		T operator()(Parameters... params) const ;
};

#endif