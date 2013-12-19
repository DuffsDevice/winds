#ifndef _WIN_L_FUNC_
#define _WIN_L_FUNC_

#include "_lua/lua.hpp"
#include "_type/type.h"
#include "_type/type.event.h"
#include "_type/type.rect.h"
#include "_type/type.font.h"
#include "_type/type.color.h"
#include "_type/type.style.h"
#include "_type/type.bitmap.h"
#include "_type/type.bitmapPort.h"
#include "_type/type.assocvector.h"
#include "_type/type.callback.h"
#include "_type/type.mime.h"
#include <type_traits>

enum class _eventCallType : _u8;
enum class _dialogResult : _u8;
extern _fromStr<_eventCallType>	string2eventCallType;
extern _toStr<_eventCallType>	eventCallType2string;
extern _toStr<_dialogResult>	dialogResult2string;

namespace _luafunc
{
	//! Call this function to indicate a runtime-error that happened inside a lua-function
	void	errorHandler( lua_State* L , const char* , ... );
	
	//! More or less helpers for the following templates
	void	pushEvent( lua_State* L , _event&& );
	void	pushRect( lua_State* L , _rect&& );
	void	pushArea( lua_State* L , _area&& );
	void	pushBitmap( lua_State* L , _bitmap&& );
	void	pushBitmapRef( lua_State* L , _bitmap& );
	void	pushBitmapPort( lua_State* L , _bitmapPort&& );
	void	pushGadget( lua_State* L , _gadget* );
	void	pushFont( lua_State* L , const _font* );
	void	pushBorder( lua_State* L , _border&& );
}

#include "_lua/lua.func.isa.h"
#include "_lua/lua.func.push.h"
#include "_lua/lua.func.check1.h"
#include "_lua/lua.func.callback1.h"
#include "_lua/lua.func.check2.h"

namespace _luafunc
{
	template<typename T>
	using remove_ref = typename std::remove_reference<T>::type;
	
	template<typename T>
	using remove_const = typename std::remove_const<T>::type;
	
	template<typename T>
	using helperType1 = typename std::enable_if< (std::is_reference<T>::value && std::is_const<remove_ref<T>>::value)>::type;
	
	template<typename T>
	using helperType2 = typename std::enable_if<(!std::is_reference<T>::value)>::type;
	
	// ~~~~~~~~~~~~~~~~~ Front-End Methods ~~~~~~~~~~~~~~~~~
	
	//! Check if a stack object is of the specified type
	template<
		typename T
		, typename T2 = remove_const< remove_ref<T> >
	>
	static unused inline bool is_a( lua_State* L , int index ){
		return _luafunc::detail::is_a( L , index , (T2*)nullptr );
	}
	
	//! Check if a stack object is of a specific class (determined by its class-name)
	static unused inline bool	is_a( lua_State* L , int index , const char* className ){
		if( !lua_isuserdata(L, index) ){			/* value is a userdata? */
			if( lua_getmetatable(L, index) ){		/* does it have a metatable? */
				luaL_getmetatable(L, className);	/* get correct metatable */
				if (lua_rawequal(L, -1, -2)){		/* does it have the correct mt? */
					lua_pop(L, 2);					/* remove both metatables */
					return true;
				}
				lua_pop(L, 2);						/* remove both metatables */
			}
		}
		return false;
	}
	
	//! check<T> for const-reference types
	template<typename T>
	static unused inline auto check( lua_State* L , int index , helperType1<T>* = nullptr )
		-> decltype( _luafunc::detail::check( L , index , (remove_const<remove_ref<T>>*)nullptr ) )
	{
		return _luafunc::detail::check( L , index , (remove_const<remove_ref<T>>*)nullptr );
	}
	
	//! check<T> for non-reference types
	template<typename T>
	static unused inline auto check( lua_State* L , int index , helperType2<T>* = nullptr )
		-> decltype( _luafunc::detail::check( L , index , (remove_const<T>*)nullptr ) )
	{
		return _luafunc::detail::check( L , index , (remove_const<T>*)nullptr );
	}
	
	//! Receive the object of type 'T' from the stack, if it doesn't exist, 'fallback' is returned
	template<
		typename T
		, typename Ret = decltype( _luafunc::check<T>( nullptr , 0 ) )
		, typename ParamType = Ret
	>
	static unused Ret lightcheck( lua_State* L , int index , ParamType&& fallback = ParamType() ){
		if( _luafunc::is_a<T>( L , index ) )
			return _luafunc::check<T>( L , index );
		return move(fallback);
	}
	
	//! Receive the object of type 'T' from the stack, if it doesn't exist, an empty _optValue is returned
	template<typename T>
	static unused _optValue< remove_ref<T> > optcheck( lua_State* L , int index ){
		if( _luafunc::is_a<T>( L , index ) )
			return _luafunc::check<T>( L , index );
		return ignore;
	}
}

#include "_lua/lua.func.callback2.h"

#endif