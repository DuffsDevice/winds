#ifndef _WIN_T_LUAFUNC_
	#include "_lua/lua.func.h"
#endif

#ifndef _WIN_L_FUNC_CHECK_
#define _WIN_L_FUNC_CHECK_

#include "_lua/lua.func.h"
#include "_lua/lua.func.isa.h"

namespace _luafunc
{
	namespace detail
	{
		// ~~~~~~~~~~~~~~~~~~ Predefines ~~~~~~~~~~~~~~~~~~
		template<typename ContainerType>	static unused ContainerType checkContainerInternal( lua_State* L , int index );
		template<typename ContainerType>	static unused ContainerType checkAssociativeContainerInternal( lua_State* L , int index );
		
		// ~~~~~~~~~~~~~~~~~~ Errors ~~~~~~~~~~~~~~~~~~
		static unused int type_error (lua_State *L, int narg, const char *tname) {
			const char *msg = lua_pushfstring(L, "%s expected, got %s",tname, luaL_typename(L, narg));
			return luaL_argerror(L, narg, msg);
		}
		static unused void tag_error (lua_State *L, int narg, int tag) {
			type_error(L, narg, lua_typename(L, tag));
		}
		
		// ~~~~~~~~~~~~~~~~~~ Basic Types ~~~~~~~~~~~~~~~~~~
		static unused inline void					check( lua_State* L , int index , void* dummy ){}
		static unused inline int					check( lua_State* L , int index , int* dummy ){ return luaL_checkint( L , index ); }
		static unused inline unsigned int			check( lua_State* L , int index , unsigned int* dummy ){ return luaL_checkint( L , index ); }
		static unused inline unsigned int			check( lua_State* L , int index , long long int* dummy ){ return luaL_checknumber( L , index ); }
		static unused inline unsigned int			check( lua_State* L , int index , unsigned long long int* dummy ){ return luaL_checkint( L , index ); }
		static unused inline short int				check( lua_State* L , int index , short int* dummy ){ return luaL_checkint( L , index ); }
		static unused inline char					check( lua_State* L , int index , char* dummy ){ return luaL_checkint( L , index ); }
		static unused inline unsigned char			check( lua_State* L , int index , unsigned char* dummy ){ return luaL_checkint( L , index ); }
		static unused inline bool					check( lua_State* L , int index , bool* dummy ){
			if( lua_isnumber(L,index) )
				return lua_tonumber(L,index);
			return lua_toboolean(L, index);
		}
		static unused inline string					check( lua_State* L , int index , string* dummy ){ return luaL_checkstring( L , index ); }
		static unused inline const char*			check( lua_State* L , int index , const char** dummy ){ return luaL_checkstring( L , index ); }
		_bitmapPort									check( lua_State* L , int index , _bitmapPort* dummy );
		_bitmap										check( lua_State* L , int index , _bitmap* dummy );
		_gadget*									check( lua_State* L , int index , _gadget** dummy );
		_rect										check( lua_State* L , int index , _rect* dummy );
		_area										check( lua_State* L , int index , _area* dummy );
		const _font*								check( lua_State* L , int index , const _font** dummy );
		static unused inline _callbackReturn		check( lua_State* L , int index , _callbackReturn* dummy ){ return string2callbackReturn[ luaL_checkstring( L , index ) ]; }
		static unused inline _eventCallType			check( lua_State* L , int index , _eventCallType* dummy ){ return string2eventCallType[ luaL_checkstring( L , index ) ]; }
		static unused inline _dimension				check( lua_State* L , int index , _dimension* dummy ){ return string2dimension[ luaL_checkstring( L , index ) ]; }
		static unused inline _style					check( lua_State* L , int index , _style* dummy ){ _style style; applyString2style( style , luaL_checkstring( L , index ) ); return style; }
		static unused inline _pixel					check( lua_State* L , int index , _pixel* dummy ){
			if( lua_isnumber( L , index ) )
				return lua_tointeger( L , index );
			return string2color[ luaL_checkstring( L , index ) ];
		}
		
		// ~~~~~~~~~~~~~~~~~~ Normal Containers ~~~~~~~~~~~~~~~~~~
		template<typename T>
		static unused inline _list<T>				check( lua_State* L , int index , _list<T>* dummy ){
			return checkContainerInternal<_list<T>>( L , index );
		}
		template<typename T>
		static unused inline _vector<T>				check( lua_State* L , int index , _vector<T>* dummy ){
			return checkContainerInternal<_vector<T>>( L , index );
		}
		
		// ~~~~~~~~~~~~~~~~~~ Associative Containers ~~~~~~~~~~~~~~~~~~
		template<typename T1, typename T2>
		static unused inline _map<T1,T2>			check( lua_State* L , int index , _map<T1,T2>* dummy ){
			return checkAssociativeContainerInternal<_map<T1,T2>>( L , index );
		}
		template<typename T1, typename T2>
		static unused inline _unorderedMap<T1,T2>	check( lua_State* L , int index , _unorderedMap<T1,T2>* dummy ){
			return checkAssociativeContainerInternal<_unorderedMap<T1,T2>>( L , index );
		}
		template<typename T1, typename T2, typename C, typename A>
		static unused _assocVector<T1,T2,C,A>		check( lua_State* L , int index , _assocVector<T1,T2,C,A>* dummy ){
			return checkAssociativeContainerInternal<_assocVector<T1,T2,C,A>>( L , index );
		}
		
		// ~~~~~~~~~~~~~~~~~~~ Internal Container Checks ~~~~~~~~~~~~~~~~~~~
		template<typename ContainerType>
		static unused ContainerType checkContainerInternal( lua_State* L , int index )
		{
			// Check if table is present
			if( !lua_istable( L , index ) )
				detail::tag_error( L , index , LUA_TTABLE );
			
			typedef typename ContainerType::value_type ValueType;
			
			ContainerType ret;
			for( int i = 1; ; i++ )
			{
				lua_rawgeti( L , index , i ); // Get next index in table
				
				if( lua_isnil( L , -1 ) ){
					lua_pop( L , 1 ); // Pop Value
					break;
				}
				ret.push_back( check( L , -1 , (ValueType*)nullptr ) );
				
				lua_pop( L , 1 );// Pop Value
			}
			return ret;
		}
		
		template<typename ContainerType>
		static unused ContainerType checkAssociativeContainerInternal( lua_State* L , int index )
		{
			// Check if table is present
			if( !lua_istable( L , index ) )
				detail::tag_error( L , index , LUA_TTABLE );
			
			typedef typename ContainerType::key_type KeyType;
			typedef typename ContainerType::mapped_type ValueType;
			
			ContainerType ret;
			for( int i = 1; ; i++ )
			{
				lua_rawgeti( L , index , i ); // Get next index in table
				
				if( lua_isnil( L , -1 ) ){
					lua_pop( L , 1 ); // Pop Value
					break;
				}
				else if( lua_istable( L , -1 ) )
				{
					lua_rawgeti( L , -1 , 1 ); // Get key
					auto key = check( L , -1 , (KeyType*)nullptr );
					lua_pop( L , 1 ); // pop key
					
					lua_rawgeti( L , -1 , 2 ); // Get value
					auto value = check( L , -1 , (ValueType*)nullptr );
					lua_pop( L , 1 ); // pop value
					
					ret[key] = value;
				}
				else
					ret[ i - 1 ] = check( L , -1 , (ValueType*)nullptr );
				
				lua_pop( L , 1 );// Pop Value
			}
			
			return ret;
		}
	}
}

#endif