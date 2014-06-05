#ifndef _WIN_T_LUAFUNC_
	#include "_lua/lua.func.h"
#endif

#ifndef _WIN_T_LUAFUNCS_IS_A_
#define _WIN_T_LUAFUNCS_IS_A_

namespace _luafunc
{
	namespace detail
	{
		// Get the type of a lua index
		static unused inline int	get_type( lua_State* state , int index ){ return lua_type( state , index ); }
		
		// ~~~~~~~~~~~~~~~~~~ Basic Types ~~~~~~~~~~~~~~~~~~
		static unused inline bool	is_a( lua_State* state , int index , int luaType ){ return get_type( state , index ) == luaType; }
		static unused inline bool	is_a( lua_State* state , int index , void* dummy){ return true; }
		static unused inline bool	is_a( lua_State* state , int index , int* dummy){ return lua_isnumber( state , index ); }
		static unused inline bool	is_a( lua_State* state , int index , unsigned int* dummy){ return lua_isnumber( state , index ); }
		static unused inline bool	is_a( lua_State* state , int index , char* dummy){ return lua_isnumber( state , index ); }
		static unused inline bool	is_a( lua_State* state , int index , unsigned char* dummy){ return lua_isnumber( state , index ); }
		static unused inline bool	is_a( lua_State* state , int index , short int* dummy){ return is_a( state , index , LUA_TNUMBER ); }
		static unused inline bool	is_a( lua_State* state , int index , unsigned short int* dummy){ return lua_isnumber( state , index ); }
		static unused inline bool	is_a( lua_State* state , int index , long long int* dummy){ return lua_isnumber( state , index ); }
		static unused inline bool	is_a( lua_State* state , int index , unsigned long long int* dummy){ return lua_isnumber( state , index ); }
		static unused inline bool	is_a( lua_State* state , int index , bool* dummy ){
			int type = get_type( state , index );
			return type == LUA_TBOOLEAN || type == LUA_TNUMBER;
		}
		static unused inline bool	is_a( lua_State* state , int index , _literal* dummy ){ return is_a( state , index , LUA_TSTRING ); }
		static unused inline bool	is_a( lua_State* state , int index , string* dummy ){ return is_a( state , index , LUA_TSTRING ); }
		template<typename... TN, int mB,typename dT>
		static unused inline bool	is_a( lua_State* state , int index , shortString<mB,dT>* dummy ){ return is_a( state , index , LUA_TSTRING ); }
		static unused inline bool	is_a( lua_State* state , int index , _key* dummy ){
			int type = get_type( state , index );
			return type == LUA_TSTRING || type == LUA_TNUMBER;
		}
		bool						is_a( lua_State* state , int index , const _font** dummy );
		bool						is_a( lua_State* state , int index , _event* dummy );
		bool						is_a( lua_State* state , int index , _rect* dummy );
		bool						is_a( lua_State* state , int index , _hardwareKeyPattern* dummy );
		bool						is_a( lua_State* state , int index , _area* dummy );
		bool						is_a( lua_State* state , int index , _bitmap* dummy );
		bool						is_a( lua_State* state , int index , _bitmapPort* dummy );
		bool						is_a( lua_State* state , int index , _border* dummy );
		bool						is_a( lua_State* state , int index , _time* dummy );
		bool						is_a( lua_State* state , int index , _mimeType* dummy );
		bool						is_a( lua_State* state , int index , _color* dummy );
		static unused inline bool	is_a( lua_State* state , int index , _gadget** dummy ){ return lua_isuserdata( state , index ); }
		static unused inline bool	is_a( lua_State* state , int index , _callbackReturn* dummy ){ return is_a( state , index , LUA_TSTRING ); }
		static unused inline bool	is_a( lua_State* state , int index , _eventCallType* dummy ){ return is_a( state , index , LUA_TSTRING ); }
		static unused inline bool	is_a( lua_State* state , int index , _eventType* dummy ){ return is_a( state , index , LUA_TSTRING ); }
		static unused inline bool	is_a( lua_State* state , int index , _dimension* dummy ){ return is_a( state , index , LUA_TSTRING ); }
		static unused inline bool	is_a( lua_State* state , int index , _style* dummy ){ return is_a( state , index , LUA_TSTRING ); }
		static unused inline bool	is_a( lua_State* state , int index , _timeAttr* dummy ){ return is_a( state , index , LUA_TSTRING ); }
		static unused inline bool	is_a( lua_State* state , int index , _align* dummy ){ return is_a( state , index , LUA_TSTRING ); }
		static unused inline bool	is_a( lua_State* state , int index , _valign* dummy ){ return is_a( state , index , LUA_TSTRING ); }
		static unused inline bool	is_a( lua_State* state , int index , _language* dummy ){ return is_a( state , index , LUA_TSTRING ); }
		static unused inline bool	is_a( lua_State* state , int index , _programArgs* dummy ){ return is_a( state , index , LUA_TSTRING ); }
		static unused inline bool	is_a( lua_State* state , int index , _imageFileCompression* dummy ){ return is_a( state , index , LUA_TSTRING ); }
		
		// ~~~~~~~~~~~~~~~~~~ Normal Containers ~~~~~~~~~~~~~~~~~~
		template<typename T>
		static unused inline bool	is_a( lua_State* state , int index , _list<T>* dummy ){ return lua_istable( state , index ); }
		template<typename T>
		static unused inline bool	is_a( lua_State* state , int index , _vector<T>* dummy ){ return lua_istable( state , index ); }
		template<typename T1, typename T2>
		static unused inline bool	is_a( lua_State* state , int index , _pair<T1,T2>* dummy ){ return lua_istable( state , index ); }
		
		// ~~~~~~~~~~~~~~~~~~ Associative Containers ~~~~~~~~~~~~~~~~~~
		template<typename T1, typename T2>
		static unused inline bool	is_a( lua_State* state , int index , _map<T1,T2>* dummy ){ return lua_istable( state , index ); }
		template<typename T1, typename T2>
		static unused inline bool	is_a( lua_State* state , int index , _unorderedMap<T1,T2>* dummy ){ return lua_istable( state , index ); }
		template<typename T1, typename T2, typename C, typename A>
		static unused inline bool	is_a( lua_State* state , int index , _assocVector<T1,T2,C,A>* dummy ){ return lua_istable( state , index ); }
		
		// ~~~~~~~~~~~~~~~~~~ Associative Containers ~~~~~~~~~~~~~~~~~~
		template<typename T>
		static unused inline bool	is_a( lua_State* state , int index , T* dummy){ return T::test( state , index ); }
	}
}

#endif