#ifndef _WIN_T_LUAFUNC_
	#include "_lua/lua.func.h"
#endif

#ifndef _WIN_L_FUNC_CHECK_1_
#define _WIN_L_FUNC_CHECK_1_

namespace _luafunc
{
	namespace detail
	{
		// ~~~~~~~~~~~~~~~~~~ Predefines ~~~~~~~~~~~~~~~~~~
		template<typename ContainerType>	static unused ContainerType checkContainerInternal( lua_State* state , int index );
		template<typename ContainerType>	static unused ContainerType checkAssociativeContainerInternal( lua_State* state , int index );
		
		// ~~~~~~~~~~~~~~~~~~ Basic Types ~~~~~~~~~~~~~~~~~~
		static unused inline void					check( lua_State* state , int index , void* dummy ){}
		static unused inline int					check( lua_State* state , int index , int* dummy ){ return luaL_checkint( state , index ); }
		static unused inline unsigned int			check( lua_State* state , int index , unsigned int* dummy ){ return luaL_checkint( state , index ); }
		static unused inline unsigned int			check( lua_State* state , int index , long long int* dummy ){ return luaL_checknumber( state , index ); }
		static unused inline unsigned int			check( lua_State* state , int index , unsigned long long int* dummy ){ return luaL_checkint( state , index ); }
		static unused inline short int				check( lua_State* state , int index , short int* dummy ){ return luaL_checkint( state , index ); }
		static unused inline char					check( lua_State* state , int index , char* dummy ){ return luaL_checkint( state , index ); }
		static unused inline unsigned char			check( lua_State* state , int index , unsigned char* dummy ){ return luaL_checkint( state , index ); }
		static unused inline bool					check( lua_State* state , int index , bool* dummy ){
			if( lua_isnumber(state,index) )
				return lua_tonumber(state,index);
			return lua_toboolean(state, index);
		}
		static unused inline string					check( lua_State* state , int index , string* dummy ){ return luaL_checkstring( state , index ); }
		static unused inline const char*			check( lua_State* state , int index , _literal* dummy ){ return luaL_checkstring( state , index ); }
		template<int mB,typename dT = char>
		static unused inline shortString<mB,dT>		check( lua_State* state , int index , shortString<mB,dT>* dummy ){ return luaL_checkstring( state , index ); }
		static unused inline _key					check( lua_State* state , int index , _key* dummy ){
			return lua_isstring( state , index ) ? (_key)lua_tostring( state , index )[0] : (_key)lua_tonumber( state , index );
		}
		_hardwareKeyPattern									check( lua_State* state , int index , _hardwareKeyPattern* dummy );
		_bitmapPort									check( lua_State* state , int index , _bitmapPort* dummy );
		_bitmap										check( lua_State* state , int index , _bitmap* dummy );
		_gadget*									check( lua_State* state , int index , _gadget** dummy );
		_rect										check( lua_State* state , int index , _rect* dummy );
		_area										check( lua_State* state , int index , _area* dummy );
		_event										check( lua_State* state , int index , _event* dummy );
		const _font*								check( lua_State* state , int index , const _font** dummy );
		_border										check( lua_State* state , int index , _border* dummy );
		_time										check( lua_State* state , int index , _time* dummy );
		static unused inline _callbackReturn		check( lua_State* state , int index , _callbackReturn* dummy ){ return string2callbackReturn[ luaL_checkstring( state , index ) ]; }
		static unused inline _eventCallType			check( lua_State* state , int index , _eventCallType* dummy ){ return string2eventCallType[ luaL_checkstring( state , index ) ]; }
		static unused inline _eventType				check( lua_State* state , int index , _eventType* dummy ){ return string2eventType[ luaL_checkstring( state , index ) ]; }
		static unused inline _dimension				check( lua_State* state , int index , _dimension* dummy ){ return string2dimension[ luaL_checkstring( state , index ) ]; }
		static unused inline _align					check( lua_State* state , int index , _align* dummy ){ return string2align[ luaL_checkstring( state , index ) ]; }
		static unused inline _valign				check( lua_State* state , int index , _valign* dummy ){ return string2valign[ luaL_checkstring( state , index ) ]; }
		static unused inline _language				check( lua_State* state , int index , _language* dummy ){ return string2language[ luaL_checkstring( state , index ) ]; }
		static unused inline _timeAttr				check( lua_State* state , int index , _timeAttr* dummy ){ return string2timeAttr[ luaL_checkstring( state , index ) ]; }
		static unused inline _style					check( lua_State* state , int index , _style* dummy ){ _style style; applyString2style( style , luaL_checkstring( state , index ) ); return style; }
		static unused inline _pixel					check( lua_State* state , int index , _pixel* dummy ){
			if( lua_isnumber( state , index ) )
				return lua_tointeger( state , index );
			return string2color[ luaL_checkstring( state , index ) ];
		}
		template<typename T>
		static unused inline _optValue<T>			check( lua_State* state , int index , _optValue<T>* dummy ){
			if( is_a( state , index , (T*)nullptr ) )
				return check( state , index , (T*)nullptr );
			return ignore;
		}
		
		// ~~~~~~~~~~~~~~~~~~ Normal Containers ~~~~~~~~~~~~~~~~~~
		template<typename T>
		static unused inline _list<T>				check( lua_State* state , int index , _list<T>* dummy ){
			return checkContainerInternal<_list<T>>( state , index );
		}
		template<typename T>
		static unused inline _vector<T>				check( lua_State* state , int index , _vector<T>* dummy ){
			return checkContainerInternal<_vector<T>>( state , index );
		}
		
		// ~~~~~~~~~~~~~~~~~~ Associative Containers ~~~~~~~~~~~~~~~~~~
		template<typename T1, typename T2>
		static unused inline _map<T1,T2>			check( lua_State* state , int index , _map<T1,T2>* dummy ){
			return checkAssociativeContainerInternal<_map<T1,T2>>( state , index );
		}
		template<typename T1, typename T2>
		static unused inline _unorderedMap<T1,T2>	check( lua_State* state , int index , _unorderedMap<T1,T2>* dummy ){
			return checkAssociativeContainerInternal<_unorderedMap<T1,T2>>( state , index );
		}
		template<typename T1, typename T2, typename C, typename A>
		static unused _assocVector<T1,T2,C,A>		check( lua_State* state , int index , _assocVector<T1,T2,C,A>* dummy ){
			return checkAssociativeContainerInternal<_assocVector<T1,T2,C,A>>( state , index );
		}
		
		// ~~~~~~~~~~~~~~~~~~~ Internal Container Checks ~~~~~~~~~~~~~~~~~~~
		template<typename ContainerType>
		static unused ContainerType checkContainerInternal( lua_State* state , int index )
		{
			// Check if table is present
			if( !lua_istable( state , index ) )
				lua_tagerror( state , index , LUA_TTABLE );
			
			typedef typename ContainerType::value_type ValueType;
			
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
		
		template<typename ContainerType>
		static unused ContainerType checkAssociativeContainerInternal( lua_State* state , int index )
		{
			// Check if table is present
			if( !lua_istable( state , index ) )
				lua_tagerror( state , index , LUA_TTABLE );
			
			typedef typename ContainerType::key_type KeyType;
			typedef typename ContainerType::mapped_type ValueType;
			
			ContainerType ret;
			for( int i = 1; ; i++ )
			{
				lua_rawgeti( state , index , i ); // Get next index in table
				
				if( lua_isnil( state , -1 ) ){
					lua_pop( state , 1 ); // Pop Value
					break;
				}
				else if( lua_istable( state , -1 ) )
				{
					lua_rawgeti( state , -1 , 1 ); // Get key
					auto key = check( state , -1 , (KeyType*)nullptr );
					lua_pop( state , 1 ); // pop key
					
					lua_rawgeti( state , -1 , 2 ); // Get value
					auto value = check( state , -1 , (ValueType*)nullptr );
					lua_pop( state , 1 ); // pop value
					
					ret[key] = value;
				}
				else
					ret[ i - 1 ] = check( state , -1 , (ValueType*)nullptr );
				
				lua_pop( state , 1 );// Pop Value
			}
			
			return ret;
		}
	}
}

#endif