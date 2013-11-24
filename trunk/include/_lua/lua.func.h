#ifndef _WIN_T_LUAFUNC_
#define _WIN_T_LUAFUNC_

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
#include <type_traits>

enum class _eventCallType : _u8;
enum class _dialogResult : _u8;
extern _map<string,_eventCallType> string2eventCallType;
extern _map<_eventCallType,string> eventCallType2string;
extern _map<_dialogResult,string> dialogResult2string;

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
}

#include "_lua/lua.func.isa.h"
#include "_lua/lua.func.check.h"
#include "_lua/lua.func.push.h"

namespace _luafunc
{
	template<typename T>
	using remove_ref = typename std::remove_reference<T>::type;
	
	template<typename T>
	using remove_const = typename std::remove_const<T>::type;
	// ~~~~~~~~~~~~~~~~~ Front-End Methods ~~~~~~~~~~~~~~~~~
	
	// Check if a stack object is of the specified type
	template<
		typename T
		, typename T2 = remove_const< remove_ref<T> >
	>
	static unused inline bool is_a( lua_State* L , int index ){
		return detail::is_a( L , index , (T2*)nullptr );
	}
	
	// Check if a stack object is of a specific class (determined by its class-name)
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
	
	// check<T> for const-reference types
	template<typename T>
		typename std::enable_if< (std::is_reference<T>::value && std::is_const<remove_ref<T>>::value) , remove_const<remove_ref<T>>>
	::type static unused inline check( lua_State* L , int index ){
		return detail::check( L , index , (remove_const<remove_ref<T>>*)nullptr );
	}
	
	// check<T> for non-reference types
	template<typename T>
		typename std::enable_if<(!std::is_reference<T>::value) , remove_const<T>>
	::type static unused inline check( lua_State* L , int index ){
		return detail::check( L , index , (remove_const<T>*)nullptr );
	}
	
	// Receive the object of type 'T' from the stack, if it doesn't exist, 'fallback' is returned
	template<typename T>
	static unused remove_ref<T> lightcheck( lua_State* L , int index , remove_ref<T>&& fallback = remove_ref<T>() ){
		if( is_a<T>( L , index ) )
			return check<T>( L , index );
		return move(fallback);
	}
	
	// Receive the object of type 'T' from the stack, if it doesn't exist, an empty _optValue is returned
	template<typename T>
	static unused _optValue< remove_ref<T> > optcheck( lua_State* L , int index ){
		if( is_a<T>( L , index ) )
			return check<T>( L , index );
		return ignore;
	}
}

#endif