#ifndef _WIN_T_LUAFUNC_
	#include "_lua/lua.func.h"
#endif

#ifndef _WIN_L_FUNC_CHECK_2_
#define _WIN_L_FUNC_CHECK_2_

namespace _luafunc
{
	namespace detail
	{
		// ~~~~~~~~~~~~~~~~~~ Callbacks ~~~~~~~~~~~~~~~~~~
		template<typename T, typename... Args>
		inline _luaCallback<T(Args...)>	check( lua_State* L , int index , _callback<T(Args...)>* dummy ){
			return _luaCallback<T(Args...)>( L , index );
		}
		template<typename T, typename... Args>
		inline bool	is_a( lua_State* L , int index , _callback<T(Args...)>* dummy ){ return lua_isfunction( L , index ); }
		
		// ~~~~~~~~~~~~~~ Parameter Allocators ~~~~~~~~~~~~~~
		template<typename T>
		static unused inline _paramAlloc<T>			check( lua_State* L , int index , _paramAlloc<T>* dummy ){
			return check( L , index , (T*)nullptr );
		}
	}
}

#endif