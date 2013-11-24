#ifndef _WIN_L_FUNC_WRAP_
#define _WIN_L_FUNC_WRAP_

#include "_type/type.h"
#include "_lua/lua.func.h"
#include "_lua/lua.hpp"
#include <type_traits>

namespace wrapHelpers
{
	template<size_t index,typename... Types>
	using nth_type
		= typename std::tuple_element<index, std::tuple<Types...>>::type;
	
	template<class ProxyClass, typename WrappedFuncType,WrappedFuncType funcToWrap, typename Return, typename Class, typename... Args>
	class wrapFunctionHelperClass
	{
		private:
			
			typedef Return (Class::*wrappedFuncType)(Args...);
			typedef int (ProxyClass::*proxyFuncType)(lua_State*);
			
			// Dispatcher for void-functions
			template<
				size_t curIndex
				, size_t maxIndex
				, class... CollectedArgs 
			>
			inline typename std::enable_if<( curIndex == maxIndex && std::is_same<Return,void>::value ),int>::type
				call_with_params( lua_State* L , ProxyClass& instance , CollectedArgs&&... params)
			{
				Class* dest = &((Class&)instance); // Converts the proxyclass to the destination class
				(dest->*funcToWrap)(params...);
				return 0;
			}
			
			// Dispatcher for non-void-returns
			template<
				size_t curIndex
				, size_t maxIndex
				, class... CollectedArgs
			>
			inline typename std::enable_if<( curIndex == maxIndex && !std::is_same<Return,void>::value ),int>::type
				call_with_params( lua_State* L , ProxyClass& instance , CollectedArgs&&... params)
			{
				Class* dest = &((Class&)instance); // Converts the proxyclass to the destination class
				(dest->*funcToWrap)(params...);
				_luafunc::push( L , (dest->*funcToWrap)(params...) );
				return 1;
			}
			
			template<
				size_t curIndex
				, size_t maxIndex
				, class... ParamTypes
				, class... CollectedArgs
			>
			inline typename std::enable_if<( curIndex < maxIndex ),int>::type
				call_with_params( lua_State* L , ProxyClass& instance , CollectedArgs&&... params )
			{
				typedef nth_type<curIndex,ParamTypes...> curType;
				return
					this->call_with_params<curIndex+1,maxIndex,ParamTypes...>(
						L
						, instance
						, std::move(params)...
						, _luafunc::check<curType>(L, curIndex+1 )
					);
			}
		
		public:
			
			// Method that will dispatch all calls to the wrapped function
			int dispatcher( lua_State* L )
			{
				asm volatile( "@dispatcher" );
				return this->call_with_params<0,sizeof...(Args), Args...>( L , *reinterpret_cast<ProxyClass*>(this) );
			}
			
			operator proxyFuncType(){
				return reinterpret_cast<proxyFuncType>(&wrapFunctionHelperClass::dispatcher);
			}
	};
	
	template<
		class ProxyClass,
		typename FuncType,
		FuncType funcToWrap,
		typename Class,
		typename Return,
		typename... Args
	>
	inline constexpr auto wrapFunctionHelper( Return (Class::*dummy)(Args...) )
		-> wrapFunctionHelperClass<ProxyClass, FuncType, funcToWrap, Return, Class, Args...>
	{
		return wrapFunctionHelperClass<ProxyClass, FuncType, funcToWrap, Return, Class, Args...>();
	}

	template<
		class ProxyClass,
		typename FuncType,
		FuncType funcToWrap,
		typename Class,
		typename Return,
		typename... Args
	>
	inline constexpr auto wrapFunctionHelper( Return (Class::*dummy)(Args...) const )
		-> wrapFunctionHelperClass<ProxyClass, FuncType, funcToWrap, Return, Class, Args...>
	{
		return wrapFunctionHelperClass<ProxyClass, FuncType, funcToWrap, Return, Class, Args...>();
	}
}

#define wrap(namespac, func) wrapHelpers::wrapFunctionHelper<namespac, decltype(func), (func)>(func)

#endif