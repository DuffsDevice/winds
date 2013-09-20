// Check if already included
#ifndef _WIN_L__DIALOGINTERFACE_
#define _WIN_L__DIALOGINTERFACE_

#include "_lua/lunar.h"
#include "_lua/lua.funcs.h"
#include "_type/type.callback.h"
#include "_type/type.dialog.h"
using namespace _luafunc;

/**
 * Proxy Classes
 */
template<class T>
class _lua_interface_dialog : public T
{
	typedef typename T::_dialog dummy;
	
	public:
		
		// Virtual Dtor
		virtual ~_lua_interface_dialog(){};
		
		template<typename... Args>
		_lua_interface_dialog( Args... params ) :
			T( move(params)... )
		{}
		
		//! execute
		int execute( lua_State* L ){ T::execute(); return 0; };
		
		//! terminate
		int terminate( lua_State* L ){ T::terminate(); return 0; };
		
		//! getIntResult
		int getIntResult( lua_State* L ){ push( L , T::getIntResult() ); return 1; }
		
		//! getStrResult
		int getStrResult( lua_State* L ){ push( L , T::getStrResult() ); return 1; }
		
		//! setCallback
		int setCallback( lua_State* L ) { T::setCallback( _luaCallback<void(_dialogResult)>( L , 1 ) ); return 1; }
		
		//! deleteCallback
		int deleteCallback( lua_State* L ){ T::deleteCallback(); return 0; };
		
		//! isRunning
		int isRunning( lua_State* L ){ push( L , T::isRunning() ); return 1; }
		
		//! Lunar !//
		static typename Lunar<_lua_interface_dialog>::FunctionType methods[];
		static typename Lunar<_lua_interface_dialog>::PropertyType properties[];
};

//! Lua-_dialog base functions
template<typename T>
typename Lunar<_lua_interface_dialog<T>>::FunctionType _lua_interface_dialog<T>::methods[] = {
	LUA_CLASS_FUNC( _lua_interface_dialog , execute ),
	LUA_CLASS_FUNC( _lua_interface_dialog , terminate ),
	LUA_CLASS_FUNC( _lua_interface_dialog , setCallback ),
	LUA_CLASS_FUNC( _lua_interface_dialog , deleteCallback ),
	LUA_CLASS_FUNC( _lua_interface_dialog , isRunning ),
	LUA_CLASS_FUNC_END
};

template<typename T>
typename Lunar<_lua_interface_dialog<T>>::PropertyType _lua_interface_dialog<T>::properties[] = {
	{ "intResult" , &_lua_interface_dialog::getIntResult , nullptr },
	{ "strResult" , &_lua_interface_dialog::getStrResult , nullptr },
	LUA_CLASS_ATTR_END
};

#endif