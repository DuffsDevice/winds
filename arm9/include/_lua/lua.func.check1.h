#ifndef _WIN_T_LUAFUNC_
	#include <_lua/lua.func.h>
#endif

#ifndef _WIN_L_FUNC_CHECK_1_
#define _WIN_L_FUNC_CHECK_1_

namespace _luafunc
{
	namespace detail
	{
		// ~~~~~~~~~~~~~~~~~~ Predefines ~~~~~~~~~~~~~~~~~~
		template<
			typename ContainerType
			, typename ValueType = typename ContainerType::value_type
		>	static unused ContainerType				checkContainerInternal( lua_State* state , int index );
		
		template<
			typename ContainerType
			, typename KeyType = typename ContainerType::key_type
			, typename ValueType = typename ContainerType::mapped_type
		>	static unused ContainerType				checkAssociativeContainerInternal( lua_State* state , int index );
		
		template<
			typename PairType
			, typename FirstType = typename PairType::first_type
			, typename SecondType = typename PairType::second_type
		>	static unused PairType					checkPairInternal( lua_State* state , int index );
		
		template<
			int countDown
			, int currentLuaIndex
			, typename CurrentType
			, typename... TupleTypes
		>	static unused inline
			_tuple<CurrentType,TupleTypes...>		checkTupleElementsInternal( lua_State* state , int index , typename std::enable_if<countDown,void*>::type = nullptr );
		
		template<
			int countDown
			, int currentLuaIndex
		>	static unused inline _tuple<>			checkTupleElementsInternal( lua_State* state , int , typename std::enable_if<!countDown,void*>::type = nullptr );
		
		
		
		// ~~~~~~~~~~~~~~~~~~ Basic Types ~~~~~~~~~~~~~~~~~~
		static unused inline void					check( lua_State* state , int index , void* ){}
		static unused inline int					check( lua_State* state , int index , int* ){ return luaL_checkinteger( state , index ); }
		static unused inline unsigned int			check( lua_State* state , int index , unsigned int* ){ return luaL_checkinteger( state , index ); }
		static unused inline long int				check( lua_State* state , int index , long int* ){ return luaL_checkinteger( state , index ); } // 'long int' is the same as 'int'
		static unused inline unsigned long int		check( lua_State* state , int index , unsigned long int* ){ return luaL_checkinteger( state , index ); } // 'long int' is the same as 'int'
		static unused inline long long int			check( lua_State* state , int index , long long int* ){ return luaL_checkinteger( state , index ); }
		static unused inline unsigned long long int	check( lua_State* state , int index , unsigned long long int* ){ return luaL_checkinteger( state , index ); }
		static unused inline short int				check( lua_State* state , int index , short int* ){ return luaL_checkinteger( state , index ); }
		static unused inline unsigned short int		check( lua_State* state , int index , unsigned short int* ){ return luaL_checkinteger( state , index ); }
		static unused inline unsigned char			check( lua_State* state , int index , unsigned char* ){ return luaL_checkinteger( state , index ); }
		static unused inline signed char			check( lua_State* state , int index , signed char* ){ return luaL_checkinteger( state , index ); }
		static unused inline signed char			check( lua_State* state , int index , float* ){ return luaL_checknumber( state , index ); }
		static unused inline signed char			check( lua_State* state , int index , double* ){ return luaL_checknumber( state , index ); }
		static unused inline bool					check( lua_State* state , int index , bool* ){
			if( lua_isboolean( state , index ) )
				return lua_toboolean( state , index );
			return luaL_checkinteger( state , index );
		}
		static unused inline char					check( lua_State* state , int index , char* ){
			if( lua_isstring( state , index ) )
				return luaL_checkstring( state , index )[0];
			return luaL_checkinteger( state , index );
		}
		static unused inline char					check( lua_State* state , int index , _wchar* ){
			if( lua_isstring( state , index ) )
				return wstring( luaL_checkstring( state , index ) , 6 ).front();
			return luaL_checkinteger( state , index );
		}
		static unused inline string					check( lua_State* state , int index , string* ){ _literal str = luaL_checkstring( state , index ); return str ? string(str) : string(); }
		static unused inline wstring				check( lua_State* state , int index , wstring* ){ _literal str = luaL_checkstring( state , index ); return str ? wstring(str) : wstring(); }
		static unused inline _literal				check( lua_State* state , int index , _literal* ){ return luaL_checkstring( state , index ); }
		template<int mB,typename dT = char>
		static unused inline _shortString<mB,dT>	check( lua_State* state , int index , _shortString<mB,dT>* ){ return luaL_checkstring( state , index ); }
		static unused inline _args			check( lua_State* state , int index , _args* ){ return _args( luaL_checkstring( state , index ) ); }
		static unused inline _key					check( lua_State* state , int index , _key* ){
			return is_a( state , index , LUA_TSTRING ) ? (_key)lua_tostring( state , index )[0] : (_key)luaL_checkinteger( state , index );
		}
		_hardwareKeyPattern							check( lua_State* state , int index , _hardwareKeyPattern* );
		_bitmapPort									check( lua_State* state , int index , _bitmapPort* );
		_bitmap										check( lua_State* state , int index , _bitmap* );
		_gadget*									check( lua_State* state , int index , _gadget** );
		_rect										check( lua_State* state , int index , _rect* );
		_area										check( lua_State* state , int index , _area* );
		_event										check( lua_State* state , int index , _event* );
		_fontHandle									check( lua_State* state , int index , _fontHandle* );
		_border										check( lua_State* state , int index , _border* );
		_time										check( lua_State* state , int index , _time* );
		_mimeType									check( lua_State* state , int index , _mimeType* );
		_menu										check( lua_State* state , int index , _menu* );
		_color										check( lua_State* state , int index , _color* );
		static unused inline _callbackReturn		check( lua_State* state , int index , _callbackReturn* ){ return string2callbackReturn[ luaL_checkstring( state , index ) ]; }
		static unused inline _eventCallType			check( lua_State* state , int index , _eventCallType* ){ return string2eventCallType[ luaL_checkstring( state , index ) ]; }
		static unused inline _eventType				check( lua_State* state , int index , _eventType* ){ return string2eventType[ luaL_checkstring( state , index ) ]; }
		static unused inline _dimension				check( lua_State* state , int index , _dimension* ){ return string2dimension[ luaL_checkstring( state , index ) ]; }
		static unused inline _direction				check( lua_State* state , int index , _direction* ){ return string2direction[ luaL_checkstring( state , index ) ]; }
		static unused inline _align					check( lua_State* state , int index , _align* ){ return string2align[ luaL_checkstring( state , index ) ]; }
		static unused inline _valign				check( lua_State* state , int index , _valign* ){ return string2valign[ luaL_checkstring( state , index ) ]; }
		static unused inline _language				check( lua_State* state , int index , _language* ){ return luaL_checkstring( state , index ); }
		static unused inline _timeAttr				check( lua_State* state , int index , _timeAttr* ){ return string2timeAttr[ luaL_checkstring( state , index ) ]; }
		static unused inline _imageFileCompression	check( lua_State* state , int index , _imageFileCompression* ){ return string2imageFileCompression[ luaL_checkstring( state , index ) ]; }
		static unused inline _style					check( lua_State* state , int index , _style* ){ _style style; applyString2style( style , luaL_checkstring( state , index ) ); return style; }
		template<typename T>
		static unused inline _optValue<T>			check( lua_State* state , int index , _optValue<T>* ){
			if( is_a( state , index , (T*)nullptr ) )
				return check( state , index , (T*)nullptr );
			return ignore;
		}
		template<typename SingleType,typename DoubleType>
		static unused inline
			_2T<SingleType,DoubleType>				check( lua_State* state , int index , _2T<SingleType,DoubleType>* ){
			return _2T<SingleType,DoubleType>( check( state , index , (SingleType*)nullptr ) , check( state , index + 1 , (SingleType*)nullptr ) );
		}
		
		
		
		// ~~~~~~~~~~~~~~~~~~ Tuples ~~~~~~~~~~~~~~~~~~
		template<typename... T>
		static unused inline _tuple<T...>			check( lua_State* state , int index , _tuple<T...>* arg )
		{
			// Check if table is present
			if( !lua_istable( state , index ) )
				lua_tagerror( state , index , LUA_TTABLE );
			
			return checkTupleElementsInternal<sizeof...(T),1,T...>( state , index );
		}
		static unused inline _menuEntry				check( lua_State* state , int index , _menuEntry* arg ){
			if( lua_istable( state , index ) )
				return _menuEntry( check( state , index , (_tuple<wstring,_u16>*)nullptr ) );
			return _menuEntry( check( state , index , (wstring*)nullptr) );
		}
		template<
			int countDown
			, int currentLuaIndex
			, typename CurrentType
			, typename... TupleTypes
		>
		static unused inline
			_tuple<CurrentType,TupleTypes...>		checkTupleElementsInternal( lua_State* state , int index , typename std::enable_if<countDown,void*>::type = nullptr )
		{
			lua_rawgeti( state , index , currentLuaIndex ); // Get element at current index in table
			
			// Read the current index from the lua table
			CurrentType current = check( state , -1 , (CurrentType*)nullptr );
			
			// Pop Value
			lua_pop( state , 1 );
			
			// Return concatenated tuple
			return std::tuple_cat(
				make_tuple( move(current) )
				, checkTupleElementsInternal<
					countDown - 1
					, currentLuaIndex + 1
					, TupleTypes...
				>( state , index )
			);
		}
		template<
			int countDown
			, int currentLuaIndex
		>
		static unused inline _tuple<>				checkTupleElementsInternal( lua_State* state , int , typename std::enable_if<!countDown,void*>::type = nullptr )
		{
			return _tuple<>();
		}
		
		
		
		// ~~~~~~~~~~~~~~~~~~ Normal Containers ~~~~~~~~~~~~~~~~~~
		template<typename T>
		static unused inline _list<T>				check( lua_State* state , int index , _list<T>* ){
			return checkContainerInternal<_list<T>>( state , index );
		}
		template<typename T>
		static unused inline _vector<T>				check( lua_State* state , int index , _vector<T>* ){
			return checkContainerInternal<_vector<T>>( state , index );
		}
		template<typename T1, typename T2>
		static unused inline _pair<T1,T2>			check( lua_State* state , int index , _pair<T1,T2>* ){
			return checkPairInternal<_pair<T1,T2>>( state , index );
		}
		
		
		
		// ~~~~~~~~~~~~~~~~~~ Associative Containers ~~~~~~~~~~~~~~~~~~
		template<typename T1, typename T2>
		static unused inline _map<T1,T2>			check( lua_State* state , int index , _map<T1,T2>* ){
			return checkAssociativeContainerInternal<_map<T1,T2>>( state , index );
		}
		template<typename T1, typename T2>
		static unused inline _unorderedMap<T1,T2>	check( lua_State* state , int index , _unorderedMap<T1,T2>* ){
			return checkAssociativeContainerInternal<_unorderedMap<T1,T2>>( state , index );
		}
		template<typename T1, typename T2, typename C, typename A>
		static unused _assocVector<T1,T2,C,A>		check( lua_State* state , int index , _assocVector<T1,T2,C,A>* ){
			return checkAssociativeContainerInternal<_assocVector<T1,T2,C,A>>( state , index );
		}
		
		
		
		// ~~~~~~~~~~~~~~~~~~~ Internal Container Checks ~~~~~~~~~~~~~~~~~~~
		template<
			typename ContainerType
			, typename ValueType = typename ContainerType::value_type
		>
		static unused ContainerType checkContainerInternal( lua_State* state , int index )
		{
			// Check if table is present
			if( !lua_istable( state , index ) )
				lua_tagerror( state , index , LUA_TTABLE );
			
			// Convert all relative indices to absolute since they would be invalidated after lua_rawgeti
			index = lua_toAbsIndex( state , index );
			
			ContainerType ret;
			for( int i = 1; ; i++ )
			{
				lua_rawgeti( state , index , i ); // Get next index in table
				
				if( lua_isnil( state , -1 ) ){
					lua_pop( state , 1 ); // Pop Value
					break;
				}
				ret.push_back( check( state , -1 , (ValueType*)nullptr ) );
				
				lua_pop( state , 1 );// Pop Value
			}
			return ret;
		}
		
		template<
			typename ContainerType
			, typename KeyType = typename ContainerType::key_type
			, typename ValueType = typename ContainerType::mapped_type
		>
		static unused ContainerType checkAssociativeContainerInternal( lua_State* state , int index )
		{
			// Check if table is present
			if( !lua_istable( state , index ) )
				lua_tagerror( state , index , LUA_TTABLE );
			
			// Convert all relative indices to absolute since they would be invalidated after lua_rawgeti
			index = lua_toAbsIndex( state , index );
			
			ContainerType ret;
			
			lua_pushnil( state );  // first key
			
			while( lua_next( state , index ) != 0 )
			{
				// Copy key, in case some function calls lua_tostring, which would break the lua_next-iteration
				lua_pushvalue( state , -2 );
				
				// uses copy of 'key' (at index -1) and 'value' (at index -2)
				ret[ check( state , -1 , (KeyType*)nullptr ) ] = check( state , -2 , (ValueType*)nullptr );
				
				// removes 'value' and the copy of 'key' ; keeps original 'key' for next iteration
				lua_pop( state , 2 );
			}
			
			return ret;
		}
		
		template<
			typename PairType
			, typename FirstType = typename PairType::first_type
			, typename SecondType = typename PairType::second_type
		>
		static unused PairType checkPairInternal( lua_State* state , int index )
		{
			// Check if table is present
			if( !lua_istable( state , index ) )
				lua_tagerror( state , index , LUA_TTABLE );
			
			// Convert all relative indices to absolute since they would be invalidated after lua_rawgeti
			index = lua_toAbsIndex( state , index );
			
			lua_rawgetfield( state , index , "second" ); // Get second value
			
			#ifdef _WIN_CONFIG_LUA_PAIR_AS_ARRAY_
			// Check if the pair consists of .second and .first
			if( lua_isnil( state , -1 ) ){
				lua_pop( state , 1 );
				lua_rawgeti( state , index , 2 );	// Get second value
				lua_rawgeti( state , index , 1 );	// Get first value
			}
			else
			#endif
				lua_rawgetfield( state , index , "first" );		// Get first value
			
			// [-1] = First
			// [-2] = Second
			PairType ret = PairType(
				check( state , -1 , (FirstType*)nullptr ) ,
				check( state , -2 , (SecondType*)nullptr )
			);
			
			// Pop first/second value
			lua_pop( state , 2 );
			
			return move(ret);
		}
	}
}

#endif