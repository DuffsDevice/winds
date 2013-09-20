#ifndef _WIN_T_LUAFUNCS_
#define _WIN_T_LUAFUNCS_

#include "_lua/lua.hpp"
#include "_type/type.h"
#include "_type/type.event.h"
#include "_type/type.callback.h"
#include "_type/type.color.h"
#include "_type/type.style.h"
#include "_type/type.bitmap.h"
#include <type_traits>

enum class _eventCallType : _u8;
enum class _dialogResult : _u8;
extern _map<string,_eventCallType> string2eventCallType;
extern _map<_eventCallType,string> eventCallType2string;

namespace _luafunc
{	
	void pushEvent( lua_State* L , _event&& );
	void pushGadget( lua_State* L , _gadget* );
	
	// Checks for more or less any important data type and, if it's absent, returns an invalid optValue
	template<class T> inline _optValue<T>	optcheck( lua_State* L , int index ){ return _optValue<T>(); }
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	template<> inline _optValue<int>		optcheck<int>( lua_State* L , int index ){ return lua_isnumber( L , index ) ? _optValue<int>( lua_tonumber( L , index ) ) : _optValue<int>(); }
	template<> inline _optValue<bool>		optcheck<bool>( lua_State* L , int index ){
		if( lua_isboolean(L,index) )
			return lua_toboolean(L,index);
		if( lua_isnumber(L,index) )
			return lua_tonumber(L,index);
		return _optValue<bool>();
	}
	template<> inline _optValue<string>		optcheck<string>( lua_State* L , int index ){ return lua_isstring( L , index ) ? _optValue<string>( lua_tostring( L , index ) ) : _optValue<string>(); }
	
	// Check for any type of data
	template<class T> inline T			check( lua_State* L , int index ){ return T(); }
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	template<> inline int				check<int>( lua_State* L , int index ){ return luaL_checkint( L , index ); }
	template<> inline bool				check<bool>( lua_State* L , int index ){
		if( lua_isnumber(L,index) )
			return lua_tonumber(L,index);
		return lua_toboolean(L, index);
	}
	template<> inline string			check<string>( lua_State* L , int index ){ return luaL_checkstring( L , index ); }
	template<> inline _bitmap			check<_bitmap>( lua_State* L , int index );
	template<> inline _callbackReturn	check<_callbackReturn>( lua_State* L , int index ){ return string2callbackReturn[ luaL_checkstring( L , index ) ]; }
	template<> inline _eventCallType	check<_eventCallType>( lua_State* L , int index ){ return string2eventCallType[ luaL_checkstring( L , index ) ]; }
	template<> inline _dimension		check<_dimension>( lua_State* L , int index ){ return string2dimension[ luaL_checkstring( L , index ) ]; }
	template<> inline _style			check<_style>( lua_State* L , int index ){ _style style; applyString2style( style , luaL_checkstring( L , index ) ); return style; }
	template<> inline _pixel			check<_pixel>( lua_State* L , int index ){
		if( lua_isstring( L , index ) )
			return string2color[ lua_tolstring( L , index , nullptr ) ];
		return lua_tointeger( L , index );
	}
	inline int							checkfunction( lua_State* L , int index )
	{
		if( L && lua_isfunction( L , index ) )
		{
			lua_pushvalue( L , index ); // Copy
			return luaL_ref( L , LUA_REGISTRYINDEX );
		}
		return LUA_NOREF;
	}

	template<class T> inline T			lightcheck( lua_State* L , int index , T&& fallback ){ return fallback; }
	template<class T> inline T*			lightcheck( lua_State* L , int index , T* fallback ){ return fallback; }
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	template<> inline int				lightcheck<int>( lua_State* L , int index , int&& fallback ){ return luaL_optint( L , index , fallback ); }
	template<> inline bool				lightcheck<bool>( lua_State* L , int index , bool&& fallback ){
		if( lua_isboolean(L,index) )
			return lua_toboolean(L,index);
		if( lua_isnumber(L,index) )
			return lua_tonumber(L,index);
		return fallback;
	}
	template<> inline string			lightcheck<string>( lua_State* L , int index , string&& fallback ){ return luaL_optstring( L , index , fallback.c_str() ); }
	template<> _gadget*					lightcheck<_gadget>( lua_State* L , int index , _gadget* fallback );
	template<> _bitmap					lightcheck<_bitmap>( lua_State* L , int index , _bitmap&& fallback );
	template<> inline _callbackReturn	lightcheck<_callbackReturn>( lua_State* L , int index , _callbackReturn&& fallback ){ return lua_isstring( L , index ) ? string2callbackReturn[luaL_checkstring(L, index)] : fallback; }
	template<> inline _eventCallType	lightcheck<_eventCallType>( lua_State* L , int index , _eventCallType&& fallback ){ return lua_isstring( L , index ) ? string2eventCallType[ luaL_checkstring( L , index ) ] : fallback; }
	template<> inline _dimension		lightcheck<_dimension>( lua_State* L , int index , _dimension&& fallback ){ return lua_isstring( L , index ) ? string2dimension[luaL_checkstring(L, index)] : fallback; }
	template<> inline _style			lightcheck<_style>( lua_State* L , int index , _style&& style ){ if( lua_isstring( L , index ) ) applyString2style( style , luaL_checkstring( L , index ) ); return (_style&&)style; }
	template<typename T1, typename T2> _map<T1,T2>
										lightcheck( lua_State* L , int index , _map<T1,T2>&& fallback )
	{
		// Pass Table
		if( !lua_istable( L , index ) || !std::is_convertible<int,T1>::value )
			return fallback;
		
		for( int i = 1; ; i++ )
		{
			lua_rawgeti( L , index , i ); // Get next index in table
			
			if( lua_isnil( L , -1 ) ){
				lua_pop( L , 1 ); // Pop Value
				break;
			}
			fallback[ i - 1 ] = check<T2>( L , -1 );
			
			lua_pop( L , 1 );// Pop Value
		}
		
		return fallback;
	}
	template<typename T> inline _list<T>
										lightcheck( lua_State* L , int index , _list<T>&& fallback )
	{
		// Pass Table
		if( !lua_istable( L , index ) )
			return fallback;
		
		for( int i = 1; ; i++ )
		{
			lua_rawgeti( L , index , i ); // Get next index in table
			
			if( lua_isnil( L , -1 ) ){
				lua_pop( L , 1 ); // Pop Value
				break;
			}
			fallback.push_back( check<T>( L , -1 ) );
			
			lua_pop( L , 1 );// Pop Value
		}
		return fallback;
	}
	template<typename T> inline _vector<T>
										lightcheck( lua_State* L , int index , _vector<T>&& fallback )
	{
		// Pass Table
		if( !lua_istable( L , index ) )
			return fallback;
		
		for( int i = 1; ; i++ )
		{
			lua_rawgeti( L , index , i ); // Get next index in table
			
			if( lua_isnil( L , -1 ) ){
				lua_pop( L , 1 ); // Pop Value
				break;
			}
			fallback.push_back( check<T>( L , -1 ) );
			
			lua_pop( L , 1 );// Pop Value
		}
		return fallback;
	}
	template<> inline _pixel			lightcheck<_pixel>( lua_State* L , int index , _pixel&& fallback ){
		if( lua_isstring( L , index ) )
			return string2color[ lua_tolstring( L , index , nullptr ) ];
		if( lua_isnumber( L , index ) )
			return lua_tointeger( L , index );
		return fallback;
	}

	//
	// Push any type of data
	//
	inline int push(lua_State*){
		return 0;
	}

	template<typename... TN>
	inline int push(lua_State* L, int arg, TN... args){
		lua_pushnumber( L , arg );
		return push( L , forward<TN>(args)... ) + 1;
	}

	template<typename... TN>
	inline int push(lua_State* L, const string& arg, TN... args){
		lua_pushstring( L , arg.c_str() );
		return push( L , forward<TN>(args)... ) + 1;
	}

	template<typename... TN>
	inline int push(lua_State* L, const char* arg, TN... args){
		lua_pushstring( L , arg );
		return push( L , forward<TN>(args)... ) + 1;
	}
	
	template<typename... TN>
	inline int push(lua_State* L, _dimension arg, TN... args){
		lua_pushstring( L , dimension2string[arg].c_str() );
		return push( L , forward<TN>(args)... ) + 1;
	}

	template<typename... TN>
	inline int push(lua_State* L, _event&& arg , TN... args){
		pushEvent( L , move(arg) );
		return push( L , forward<TN>(args)... ) + 1;
	}
	
	template<typename... TN>
	inline int push(lua_State* L, const _event& arg , TN... args){
		return push( L , _event(arg) , forward<TN>(args)... );
	}
	
	template<typename... TN>
	inline int push(lua_State* L, _dialogResult arg , TN... args){
		return push( L , arg , forward<TN>(args)... );
	}

	template<typename T, typename... TN>
	int push( lua_State* L , const _vector<T>& arg , TN... args )
	{
		// Create Table for arguments
		lua_createtable( L , arg.size() , 0 );
		int top = lua_gettop( L );
		
		// Push Datasets
		int i = 0;
		for( const T& data : arg ){
			push( L , data );
			lua_rawseti( L , top , ++i );
		}
		
		// Process left over parameters
		return push( L , forward<TN>(args)... ) + 1;
	}

	template<typename T, typename... TN>
	int push( lua_State* L , const _list<T>& arg , TN... args )
	{
		// Create Table for arguments
		lua_createtable( L , arg.size() , 0 );
		int top = lua_gettop( L );
		
		// Push Datasets
		int i = 0;
		for( const T& data : arg ){
			push( L , data );
			lua_rawseti( L , top , ++i );
		}
		
		// Process left over parameters
		return push( L , forward<TN>(args)... ) + 1;
	}
	
	template<typename T, typename T2, typename... TN>
	int push( lua_State* L , const _map<T,T2>& arg , TN... args )
	{
		// Create Table for arguments
		lua_createtable( L , arg.size() , 0 );
		int top = lua_gettop( L );
		
		// Push Datasets
		int i = 0;
		for( const typename _map<T,T2>::value_type& data : arg )
		{
			// Create table for [1] = key and [2] = value
			lua_createtable( L , 2 , 0 );
			
			// Push Key
			push( L , data.first );
			lua_rawseti( L , -2 , 1 ); 
			
			// Push Value
			push( L , data.second );
			lua_rawseti( L , -2 , 2 );
			
			// Push that pair onto table
			lua_rawseti( L , top , ++i );
		}
		
		// Process left over parameters
		return push( L , forward<TN>(args)... ) + 1;
	}
	
	template<typename... TN>
	inline int push( lua_State* L , _gadget* arg , TN... args )
	{
		pushGadget( L , arg );
		
		// Process left over parameters
		return push( L , forward<TN>(args)... ) + 1;
	}
}
#endif