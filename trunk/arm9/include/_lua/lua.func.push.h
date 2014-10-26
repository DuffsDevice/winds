#ifndef _WIN_T_LUAFUNC_
	#include <_lua/lua.func.h>
#endif

#ifndef _WIN_L_FUNC_PUSH_
#define _WIN_L_FUNC_PUSH_

#include <_lua/lua.func.h>

namespace _luafunc
{
	// ~~~~~~~~~~~~~~~~~~ Predefines ~~~~~~~~~~~~~~~~~~
	template<int currentIndex, typename... ValueType> void	pushTupleElementsInternal( lua_State* state , const _tuple<ValueType...>& arg , typename std::enable_if<currentIndex,void*>::type = nullptr );
	template<int currentIndex, typename... ValueType> void	pushTupleElementsInternal( lua_State* state , const _tuple<ValueType...>& arg , typename std::enable_if<!currentIndex,void*>::type = nullptr );
	template<typename ContainerType> void					pushContainer( lua_State* state , const ContainerType& arg );
	template<typename ContainerType> void					pushAssociativeContainer( lua_State* state , const ContainerType& arg );
	template<typename... T, typename... TN> int				push( lua_State* state , const _tuple<T...>& arg , TN... args );
	template<typename T1, typename T2, typename... TN> int	push( lua_State* state , const _pair<T1,T2>& arg , TN... args );
	
	
	
	// ~~~~~~~~~~~~~~~~~~ Basic Types ~~~~~~~~~~~~~~~~~~
	inline int push(lua_State*){ return 0; } // Standard fallback
	template<typename... TN>	inline int push( lua_State* state , decltype(std::ignore)		, TN... args){ lua_pushnil( state ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , int arg						, TN... args){ lua_pushinteger( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , unsigned int arg			, TN... args){ lua_pushinteger( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , long int arg				, TN... args){ lua_pushinteger( state , arg ); return 1 + push( state , forward<TN>(args)... ); } // 'long int' is the same as 'int'
	template<typename... TN>	inline int push( lua_State* state , long unsigned int arg		, TN... args){ lua_pushinteger( state , arg ); return 1 + push( state , forward<TN>(args)... ); } // 'long int' is the same as 'int'
	template<typename... TN>	inline int push( lua_State* state , char arg					, TN... args){ lua_pushinteger( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , unsigned char arg			, TN... args){ lua_pushinteger( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , signed char arg				, TN... args){ lua_pushinteger( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , short int arg				, TN... args){ lua_pushinteger( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , unsigned short int arg		, TN... args){ lua_pushinteger( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , long long int arg			, TN... args){ lua_pushinteger( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , unsigned long long int arg	, TN... args){ lua_pushinteger( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _literal arg				, TN... args){ lua_pushstring( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , bool arg					, TN... args){ lua_pushboolean( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , const string& arg			, TN... args){ lua_pushstring( state , arg.c_str() ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN, int mB,typename dT>
								inline int push( lua_State* state , _shortString<mB,dT> arg		, TN... args){ lua_pushstring( state , arg.c_str() ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN, typename ST,typename DT>
								inline int push( lua_State* state , _2T<ST,DT> arg				, TN... args){ return 1 + push( state , make_pair( move(arg.first) , move(arg.second) ) , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _dimension arg				, TN... args){ return 1 + push( state , dimension2string[arg] , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _direction arg				, TN... args){ return 1 + push( state , direction2string[arg] , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _align arg					, TN... args){ return 1 + push( state , align2string[arg] , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _valign arg					, TN... args){ return 1 + push( state , valign2string[arg] , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _language arg				, TN... args){ return 1 + push( state , arg , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _mimeType arg				, TN... args){ pushMimeType( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _event arg					, TN... args){ pushEvent( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _dialogResult arg			, TN... args){ return push( state , dialogResult2string[arg] , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _eventType arg				, TN... args){ return push( state , eventType2string[arg] , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _timeAttr arg				, TN... args){ return push( state , timeAttr2string[arg] , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _key arg					, TN... args){ return push( state , (_char)arg , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _gadget* arg				, TN... args){ pushGadget( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _rect arg					, TN... args){ pushRect( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _area	arg					, TN... args){ pushArea( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _bitmap&& arg				, TN... args){ pushBitmap( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _bitmap& arg				, TN... args){ pushBitmapRef( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , const _bitmap& arg			, TN... args){ pushBitmap( state , _bitmap(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _bitmapPort arg				, TN... args){ pushBitmapPort( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _hardwareKeyPattern arg		, TN... args){ pushHWKP( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _fontHandle arg				, TN... args){ pushFont( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _fontPtr arg				, TN... args){ pushFont( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _menu&& arg					, TN... args){ pushMenu( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _menu& arg					, TN... args){ pushMenuRef( state , arg ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , const _menu& arg			, TN... args){ pushMenu( state , _menu(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _menuEntry arg				, TN... args){ return push( state , _tuple<string,_u16>(arg) , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _time arg					, TN... args){ pushTime( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _color arg					, TN... args){ pushColor( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _border arg					, TN... args){ pushBorder( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _padding arg				, TN... args){ pushBorder( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	template<typename... TN>	inline int push( lua_State* state , _margin arg					, TN... args){ pushBorder( state , move(arg) ); return 1 + push( state , forward<TN>(args)... ); }
	
	
	
	// ~~~~~~~~~~~~~~~~~~ Optional Values ~~~~~~~~~~~~~~~~~~
	template<typename T, typename... TN>
	inline int push( lua_State* state , _optValue<T> arg , TN... args){
		if( arg.isValid() )
			return push( state , (T&&)arg , forward<TN>(args)... );
		lua_pushnil( state );
		return 1 + push( state , forward<TN>(args)... );
	}
	
	
	
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
	inline int push( lua_State* state , const _pair<T1,T2>& arg , TN... args )
	{
		lua_createtable( state , 2 , 2 );	// Create table
		push( state , move(arg.first) );	// Push First Value
		lua_rawseti( state , -2 , 1 );		// Add to table
		push( state , move(arg.second) );	// Push Second Value
		lua_rawseti( state , -2 , 2 );		// Add to table
		lua_pushliteral( state , "first" );	// Push Key of first value
		push( state , move(arg.first) );	// Push First Value
		lua_rawset( state , -3 );			// Add to table
		lua_pushliteral( state , "second" );// Push Key of second Value
		push( state , move(arg.second) );	// Push Second Value
		lua_rawset( state , -3 );			// Add to table
		
		return push( state , args... ) + 1;
	}
	
	template<typename... T, typename... TN>
	inline int push( lua_State* state , const _tuple<T...>& arg , TN... args )
	{
		lua_createtable( state , sizeof...(T) , 0 ); // Create table
		pushTupleElementsInternal<sizeof...(T)>( state , arg );
		return push( state , args... ) + 1;
	}
	
	
	
	// ~~~~~~~~~~~~~~~~~~ Internal Push-Functions ~~~~~~~~~~~~~~~~~~
	template<int currentIndex, typename... ValueType>
	inline void pushTupleElementsInternal( lua_State* state , const _tuple<ValueType...>& arg , typename std::enable_if<currentIndex,void*>::type = nullptr)
	{
		push( state , std::get<currentIndex-1>( arg ) );
		lua_rawseti( state , -2 , currentIndex );
		
		pushTupleElementsInternal<currentIndex-1>( state , arg );
	}
	template<int currentIndex, typename... ValueType>
	inline void pushTupleElementsInternal( lua_State* state , const _tuple<ValueType...>& arg , typename std::enable_if<!currentIndex,void*>::type = nullptr )
	{}
	
	template<typename ContainerType>
	void pushAssociativeContainer( lua_State* state , const ContainerType& arg )
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
	void pushContainer( lua_State* state , const ContainerType& arg )
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