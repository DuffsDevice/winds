#ifndef _WIN_L_FUNC_
#define _WIN_L_FUNC_

#include <_lua/lua.h>
#include <_type/type.h>
#include <_type/type.event.h>
#include <_type/type.rect.h>
#include <_type/type.language.h>
#include <_type/type.font.h>
#include <_type/type.color.h>
#include <_type/type.style.h>
#include <_type/type.direction.h>
#include <_type/type.bitmap.h>
#include <_type/type.bitmap.port.h>
#include <_type/type.assocvector.h>
#include <_type/type.callback.h>
#include <_type/type.mime.h>
#include <_type/type.time.h>
#include <_type/type.key.h>
#include <_type/type.shortstring.h>
#include <_type/type.program.args.h>
#include <_type/type.imagefile.h>
#include <_type/type.menu.h>
#include <type_traits>

enum class _eventCallType : _u8;
enum class _dialogResult : _u8;
extern _fromStr<_eventCallType>	string2eventCallType;
extern _toStr<_eventCallType>	eventCallType2string;
extern _toStr<_dialogResult>	dialogResult2string;

namespace _luafunc
{
	//! More or less helpers for the following templates
	void	pushEvent( lua_State* state , _event&& );
	void	pushRect( lua_State* state , _rect&& );
	void	pushArea( lua_State* state , _area&& );
	void	pushBitmap( lua_State* state , _bitmap&& );
	void	pushBitmapRef( lua_State* state , _bitmap& );
	void	pushBitmapPort( lua_State* state , _bitmapPort&& );
	void	pushGadget( lua_State* state , _gadget* );
	void	pushFont( lua_State* state , _fontHandle );
	void	pushBorder( lua_State* state , _border&& );
	void	pushTime( lua_State* state , _time&& );
	void	pushHWKP( lua_State* state , _hardwareKeyPattern&& );
	void	pushColor( lua_State* state , _color&& );
	void	pushMimeType( lua_State* state , _mimeType&& );
	void	pushMenu( lua_State* state , _menu&& );
	void	pushMenuRef( lua_State* state , _menu& );
}

#include <_lua/lua.func.isa.h>
#include <_lua/lua.func.push.h>
#include <_lua/lua.func.check1.h>
#include <_lua/lua.func.callback1.h>
#include <_lua/lua.func.check2.h>

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
	static unused inline bool is_a( lua_State* state , int index ){
		return _luafunc::detail::is_a( state , index , (T2*)nullptr );
	}
	
	//! Check if a stack object is of a specific class (determined by its class-name)
	static unused inline bool	is_a( lua_State* state , int index , _literal className )
	{
		if( lua_getmetatable( state , index ) )		// Does it have a metatable?
		{
			luaL_getmetatable( state , className );	// Get correct metatable
			if( lua_rawequal( state , -1 , -2 ) ){	// Are they equal?
				lua_pop(state, 2);					// Remove both metatables
				return true;
			}
			lua_pop(state, 2);						// Remove both metatables
		}
		return false;
	}
	
	//! check<T> for const-reference types
	template<typename T>
	static unused inline auto check( lua_State* state , int index , helperType1<T>* = nullptr )
		-> decltype( _luafunc::detail::check( state , index , (remove_const<remove_ref<T>>*)nullptr ) )
	{
		return _luafunc::detail::check( state , index , (remove_const<remove_ref<T>>*)nullptr );
	}
	
	//! check<T> for non-reference types
	template<typename T>
	static unused inline auto check( lua_State* state , int index , helperType2<T>* = nullptr )
		-> decltype( _luafunc::detail::check( state , index , (remove_const<T>*)nullptr ) )
	{
		return _luafunc::detail::check( state , index , (remove_const<T>*)nullptr );
	}
	
	//! Receive the object of type 'T' from the stack, if it doesn't exist, 'fallback' is returned
	template<
		typename T
		, typename Ret = decltype( _luafunc::check<T>( nullptr , 0 ) )
		, typename ParamType = Ret
	>
	static unused Ret lightcheck( lua_State* state , int index , ParamType&& fallback = ParamType() ){
		if( _luafunc::is_a<T>( state , index ) )
			return _luafunc::check<T>( state , index );
		else if( !lua_isnoneornil( state , index ) ) // Wrong type supplied
			lua_notnoneerror( state , index );
		return move(fallback);
	}
	
	//! Receive the object of type 'T' from the stack, if it doesn't exist, an empty _optValue is returned
	template<typename T>
	static unused _optValue< remove_ref<T> > optcheck( lua_State* state , int index ){
		if( _luafunc::is_a<T>( state , index ) )
			return _luafunc::check<T>( state , index );
		return ignore;
	}
}

#include <_lua/lua.func.callback2.h>

#endif