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
	template<typename ContainerType> void pushContainer( lua_State* state , ContainerType arg );
	template<typename ContainerType> void pushAssociativeContainer( lua_State* state , ContainerType arg );
	
	// ~~~~~~~~~~~~~~~~~~ Basic Types ~~~~~~~~~~~~~~~~~~
	inline int push(lua_State*){ return 0; } // Standard fallback
	template<typename... TN>	inline int push( lua_State* state , int arg						, TN... args){ lua_pushnumber( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , unsigned int arg			, TN... args){ lua_pushnumber( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , char arg					, TN... args){ lua_pushnumber( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , unsigned char arg			, TN... args){ lua_pushnumber( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , short int arg				, TN... args){ lua_pushnumber( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , unsigned short int arg		, TN... args){ lua_pushnumber( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , long long int arg			, TN... args){ lua_pushnumber( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , unsigned long long int arg	, TN... args){ lua_pushnumber( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , const string& arg			, TN... args){ lua_pushstring( state , arg.c_str() ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _literal arg				, TN... args){ lua_pushstring( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _dimension arg				, TN... args){ return 1 + push( state , dimension2string[arg] , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _mimeType arg				, TN... args){ return 1 + push( state , (_literal)arg , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _event arg					, TN... args){ pushEvent( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _dialogResult arg			, TN... args){ return push( state , dialogResult2string[arg] , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _eventType arg				, TN... args){ return push( state , eventType2string[arg] , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _timeAttr arg				, TN... args){ return push( state , timeAttr2string[arg] , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _key arg					, TN... args){ return push( state , (_char)arg , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _gadget* arg				, TN... args){ pushGadget( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _rect arg					, TN... args){ pushRect( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _area	arg					, TN... args){ pushArea( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _bitmap arg					, TN... args){ pushBitmap( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _bitmap& arg				, TN... args){ pushBitmapRef( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _bitmapPort arg				, TN... args){ pushBitmapPort( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _hardwareKeyPattern arg		, TN... args){ pushHWKP( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , const _font* arg			, TN... args){ pushFont( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _time arg					, TN... args){ pushTime( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _2s32 arg					, TN... args){ return push( state , arg.first ) + push( state , arg.second , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _2u32 arg					, TN... args){ return push( state , arg.first ) + push( state , arg.second , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _border arg					, TN... args){ pushBorder( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _padding arg				, TN... args){ pushBorder( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _margin arg					, TN... args){ pushBorder( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	
	// ~~~~~~~~~~~~~~~~~~ Normal Containers ~~~~~~~~~~~~~~~~~~
	template<typename T, typename... TN>
	inline int push( lua_State* state , const _vector<T>& arg , TN... args ){ pushContainer( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename T, typename... TN>
	inline int push( lua_State* state , const _list<T>& arg , TN... args ){ pushContainer( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	
	// ~~~~~~~~~~~~~~~~~~ Associative Containers ~~~~~~~~~~~~~~~~~~
	template<typename T, typename T2, typename... TN>
	inline int push( lua_State* state , const _map<T,T2>& arg , TN... args ){ pushAssociativeContainer( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TS, typename... TN>
	inline int push( lua_State* state , const _assocVector<TS...>& arg , TN... args ){ pushAssociativeContainer( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename T, typename T2, typename... TN>
	inline int push( lua_State* state , const _unorderedMap<T,T2>& arg , TN... args ){ pushAssociativeContainer( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	
	// ~~~~~~~~~~~~~~~~~~ Pair & Tuple ~~~~~~~~~~~~~~~~~~
	template<typename T1, typename T2, typename... TN>
	inline int push( lua_State* state , _pair<T1,T2> arg , TN... args )
	{
		lua_createtable( state , 0 , 2 ); // Create table
		push( state , arg.first ); // Push Key
		lua_rawseti( state , -2 , 1 );
		push( state , arg.second ); // Push Value
		lua_rawseti( state , -2 , 2 );
		return push( state , args... ) + 1;
	}
	
	template<typename... T, typename... TN>
	inline int push( lua_State* state , _tuple<T...> arg , TN... args )
	{
		lua_createtable( state , sizeof...(T) , 0 ); // Create table
		pushTupleElementsInternal( state , arg );
		return push( state , args... ) + 1;
	}
	
	// ~~~~~~~~~~~~~~~~~~ Internal Push-Functions ~~~~~~~~~~~~~~~~~~
	template<typename... ValueType>
	inline void pushTupleElementsInternal( lua_State* state , const _tuple<ValueType...>& arg , int index = 0 )
	{
		push( state , std::get<index>( arg ) );
		lua_rawseti( state , -2 , index + 1 );
		
		if( index + 1 < sizeof...(ValueType) )
			pushTupleElements( state , arg , index + 1 );
	}
	
	template<typename ContainerType>
	void pushAssociativeContainer( lua_State* state , ContainerType arg )
	{
		// Create Table for arguments
		lua_createtable( state , 0 , arg.size() );
		
		// Push Datasets
		int i = 0;
		for( const typename ContainerType::value_type& data : arg ){
			push( state , data );
			lua_rawseti( state , -2 , ++i );
		}
	}
	
	template<typename ContainerType>
	void pushContainer( lua_State* state , ContainerType arg )
	{
		// Create Table for arguments
		lua_createtable( state , arg.size() , 0 );
		
		// Push Datasets
		int i = 0;
		for( const typename ContainerType::value_type& data : arg ){
			push( state , data );
			lua_rawseti( state , -2 , ++i );
		}
	}
}

#endif