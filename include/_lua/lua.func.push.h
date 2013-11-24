#ifndef _WIN_T_LUAFUNC_
	#include "_lua/lua.func.h"
#endif

#ifndef _WIN_L_FUNC_PUSH_
#define _WIN_L_FUNC_PUSH_

#include "_lua/lua.func.h"

namespace _luafunc
{
	// ~~~~~~~~~~~~~~~~~~ Predefines ~~~~~~~~~~~~~~~~~~
	template<typename... ValueType> void pushTupleElementsInternal( lua_State* , const _tuple<ValueType...>& , int = 0 );
	template<typename ContainerType> void pushContainer( lua_State* L , ContainerType arg );
	template<typename ContainerType> void pushAssociativeContainer( lua_State* L , ContainerType arg );
	
	// ~~~~~~~~~~~~~~~~~~ Basic Types ~~~~~~~~~~~~~~~~~~
	inline int push(lua_State*){ return 0; } // Standard fallback
	template<typename... TN>	inline int push(lua_State* L, int arg						, TN... args){ lua_pushnumber( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, unsigned int arg				, TN... args){ lua_pushnumber( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, char arg						, TN... args){ lua_pushnumber( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, unsigned char arg				, TN... args){ lua_pushnumber( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, short int arg					, TN... args){ lua_pushnumber( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, unsigned short int arg		, TN... args){ lua_pushnumber( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, long long int arg				, TN... args){ lua_pushnumber( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, unsigned long long int arg	, TN... args){ lua_pushnumber( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, const string& arg				, TN... args){ lua_pushstring( L , arg.c_str() ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, const char* arg				, TN... args){ lua_pushstring( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, _dimension arg				, TN... args){ return 1 + push( L , dimension2string[arg] , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, _event&& arg					, TN... args){ pushEvent( L , move(arg) ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, const _event& arg				, TN... args){ return push( L , _event(arg) , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, _dialogResult arg				, TN... args){ return push( L , dialogResult2string[arg] , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, _gadget* arg					, TN... args){ pushGadget( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, _rect&& arg					, TN... args){ pushRect( L , move(arg) ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, _area&& arg					, TN... args){ pushArea( L , move(arg) ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, _bitmap&& arg					, TN... args){ pushBitmap( L , move(arg) ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, _bitmap& arg					, TN... args){ pushBitmapRef( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, _constbitmap& arg				, TN... args){ return push( L , _bitmap(arg) , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, _bitmapPort&& arg				, TN... args){ pushBitmapPort( L , move(arg) ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TN>	inline int push(lua_State* L, const _font* arg				, TN... args){ pushFont( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	
	// ~~~~~~~~~~~~~~~~~~ Normal Containers ~~~~~~~~~~~~~~~~~~
	template<typename T, typename... TN>
	inline int push( lua_State* L , const _vector<T>& arg , TN... args ){ pushContainer( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename T, typename... TN>
	inline int push( lua_State* L , const _list<T>& arg , TN... args ){ pushContainer( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	
	// ~~~~~~~~~~~~~~~~~~ Associative Containers ~~~~~~~~~~~~~~~~~~
	template<typename T, typename T2, typename... TN>
	inline int push( lua_State* L , const _map<T,T2>& arg , TN... args ){ pushAssociativeContainer( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename... TS, typename... TN>
	inline int push( lua_State* L , const _assocVector<TS...>& arg , TN... args ){ pushAssociativeContainer( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	template<typename T, typename T2, typename... TN>
	inline int push( lua_State* L , const _unorderedMap<T,T2>& arg , TN... args ){ pushAssociativeContainer( L , arg ); return 1 + push( L , forward<TN>(args)... ); }
	
	// ~~~~~~~~~~~~~~~~~~ Pair & Tuple ~~~~~~~~~~~~~~~~~~
	template<typename T1, typename T2, typename... TN>
	inline int push( lua_State* L , _pair<T1,T2> arg , TN... args )
	{
		lua_createtable( L , 0 , 2 ); // Create table
		push( L , arg.first ); // Push Key
		lua_rawseti( L , -2 , 1 );
		push( L , arg.second ); // Push Value
		lua_rawseti( L , -2 , 2 );
		return push( L , args... ) + 1;
	}
	
	template<typename... T, typename... TN>
	inline int push( lua_State* L , _tuple<T...> arg , TN... args )
	{
		lua_createtable( L , sizeof...(T) , 0 ); // Create table
		pushTupleElementsInternal( L , arg );
		return push( L , args... ) + 1;
	}
	
	// ~~~~~~~~~~~~~~~~~~ Internal Push-Functions ~~~~~~~~~~~~~~~~~~
	template<typename... ValueType>
	inline void pushTupleElementsInternal( lua_State* L , const _tuple<ValueType...>& arg , int index = 0 )
	{
		push( L , std::get<index>( arg ) );
		lua_rawseti( L , -2 , index + 1 );
		
		if( index + 1 < sizeof...(ValueType) )
			pushTupleElements( L , arg , index + 1 );
	}
	
	template<typename ContainerType>
	void pushAssociativeContainer( lua_State* L , ContainerType arg )
	{
		// Create Table for arguments
		lua_createtable( L , 0 , arg.size() );
		
		// Push Datasets
		int i = 0;
		for( const typename ContainerType::value_type& data : arg ){
			push( L , data );
			lua_rawseti( L , -2 , ++i );
		}
	}
	
	template<typename ContainerType>
	void pushContainer( lua_State* L , ContainerType arg )
	{
		// Create Table for arguments
		lua_createtable( L , arg.size() , 0 );
		
		// Push Datasets
		int i = 0;
		for( const typename ContainerType::value_type& data : arg ){
			push( L , data );
			lua_rawseti( L , -2 , ++i );
		}
	}
}

#endif