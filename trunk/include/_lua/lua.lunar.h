// Check if already included
#ifndef _WIN_LUNAR_
#define _WIN_LUNAR_

#include "_type/type.h"
#include "_type/type.style.h"
#include "_lua/lua.hpp"
#include "_lua/lua.func.h"
#include <string.h> // For strlen
#include <type_traits>

namespace detail
{
	template <typename T>
	class hasStaticMethods{
		typedef char true_value;
		typedef long false_value;
		template <typename C> static true_value test( decltype(&C::staticmethods) ) ;
		template <typename C> static false_value test(...);
	public:
		enum { value = sizeof(test<T>(0)) == sizeof(true_value) };
	};
}


template<class T>
class Lunar
{
	private:
		
		// Will be enabled
		template<typename T2>
		static typename std::enable_if<detail::hasStaticMethods<T2>::value>::type registerStatics(lua_State* L)
		{
			// Create table that will hold all static methods
			lua_createtable(L,0,1);
			
			int tableIndex = lua_gettop(L);
			
			for (int i = 0; T::staticmethods[i].name; i++)
			{
				lua_pushstring(L, T2::staticmethods[i].name); // Push Key
				lua_pushcfunction(L, T2::staticmethods[i].func ); // Push Value
				lua_rawset(L, tableIndex);
			}
			
			// Create table that will become our metatable
				lua_newtable(L);
				int metatableIndex = lua_gettop(L);
				
				// Push constructor
				lua_pushliteral(L, "__call");
				lua_pushcfunction(L, &Lunar<T2>::constructorDeleteSelfReference);
				lua_settable(L, metatableIndex);
			//
			
			// Set metatable of table and pop it from stack
			lua_setmetatable(L, tableIndex);
			
			// Adds the currently created table to the global namespace and pops it from stack
			lua_setglobal(L, T2::className);
		}
		// Fallback
		template<typename T2>
		static typename std::enable_if<!detail::hasStaticMethods<T2>::value>::type registerStatics(lua_State* L)
		{
			lua_pushcfunction(L, &Lunar<T2>::constructor);
			lua_setglobal(L, T2::className);
		}
		
	public:
		
		struct PropertyType {
			const char	*name;
			int			(T::*getter)(lua_State*);
			int			(T::*setter)(lua_State*);
		};

		struct FunctionType {
			const char	*name;
			int			(T::*func)(lua_State*);
		};
		
		struct StaticType {
			const char	*name;
			int	 		(*func)(lua_State*);
		};

		/*
			Description:
			Retrieves a wrapped class from the arguments passed to the func, specified by narg (position).
			This func will raise an exception if the argument is not of the correct type.
		*/
		static T* check(lua_State* L, int narg)
		{
			T** obj = static_cast<T**>( luaL_checkudata(L, narg, T::className) );
			return *obj;		// pointer to T object
		}

		/*
			Description:
			Retrieves a wrapped class from the arguments passed to the func, specified by narg (position).
			This func will return nullptr if the argument is not of the correct type.  Useful for supporting
			multiple types of arguments passed to the func
		*/ 
		static T* lightcheck(lua_State* L, int narg) {
			T** obj = static_cast<T**>( luaL_testudata(L, narg, T::className) );
			if ( !obj )
				return nullptr; // lightcheck returns nullptr if not found.
			return *obj;		// pointer to T object
		}
		
		/*
			Description:
			Registers your class with Lua.  Leave namespac "" if you want to load it into the global space.
		*/
		static void Register( lua_State* L /*, const char *namespac = NULL*/ ) {
			
			/// This is just uncommented because of Memory usage
			//if ( namespac && strlen(namespac) )
			//{
			//	lua_getglobal(L, namespac);
			//	if( lua_isnil(L,-1) ) // Create namespace if not present
			//	{
			//		lua_newtable(L);
			//		lua_pushvalue(L,-1); // Duplicate table pointer since setglobal pops the value
			//		lua_setglobal(L,namespac);
			//	}
			//	lua_pushcfunction(L, &Lunar < T >::constructor);
			//	lua_setfield(L, -2, T::className);
			//	lua_pop(L, 1);
			//} else {
				registerStatics<T>(L);
			//}
			
			luaL_newmetatable(L, T::className);
			int             metatable = lua_gettop(L);
			
			lua_pushliteral(L, "__gc");
			lua_pushcfunction(L, &Lunar < T >::gc_obj);
			lua_settable(L, metatable);
			
			lua_pushliteral(L, "__tostring");
			lua_pushcfunction(L, &Lunar < T >::to_string);
			lua_settable(L, metatable);
			
			lua_pushliteral(L, "dump");
			lua_pushcfunction(L, &Lunar < T >::function_dump);
			lua_settable(L, metatable);
			
			lua_pushliteral(L, "__index");
			lua_pushcfunction(L, &Lunar < T >::property_getter);
			lua_settable(L, metatable);
			
			lua_pushliteral(L, "__newindex");
			lua_pushcfunction(L, &Lunar < T >::property_setter);
			lua_settable(L, metatable);
			
			for (int i = 0; T::properties[i].name; i++)
			{
				// Register some properties in it
				// Having some string associated with them
				lua_pushstring(L, T::properties[i].name );
				
				// And a number indexing which property it is
				lua_pushnumber(L, i);
				lua_rawset(L, metatable);
			}
			for (int i = 0; T::methods[i].name; i++)
			{
				// Register some functions in it
				// Having some string associated with them
				lua_pushstring(L, T::methods[i].name );
				
				// Add a number indexing which func it is
				lua_pushnumber(L, i | ( 1 << 8 ) );
				lua_rawset(L, metatable);
			}
		}
		
		/*
			constructorthat removes the 'self' reference that gets passed along in case the function is actually a metamethod
			Arguments:
			* L - Lua State
		*/
		static int constructorDeleteSelfReference(lua_State* L)
		{
			lua_remove(L,1); 
			return constructor(L);
		}
		
		static int constructor(lua_State* L)
		{
			T*  ap = new T(L);
			T** a = static_cast<T**>(lua_newuserdata(L, sizeof(T *))); // Push value = userdata
			*a = ap;
			
			luaL_getmetatable(L, T::className); 		// Fetch global metatable T::classname
			lua_setmetatable(L, -2);
			return 1;
		}
		
		/*
			Description:
			Loads an instance of the class into the Lua stack, and provides you a pointer so you can modify it.
		*/
		static void push(lua_State* L, T* instance )
		{
			if( !instance )
				return;
			
			T **a = (T **) lua_newuserdata(L, sizeof(T *)); // Create userdata
			*a = instance;
			
			luaL_getmetatable(L, T::className);
			
			lua_setmetatable(L, -2);
		}
		
		/*
			property_getter (internal)
			Arguments:
			* L - Lua State
		*/
		static int property_getter(lua_State* L)
		{
			// Object is at 1
			// Name is at 2
			lua_getmetatable(L, 1); // Look up the index of a name
			lua_pushvalue(L, 2);	// Push the name
			lua_rawget(L, -2);		// Get the index
			// Top has _index
			
			if ( lua_isnumber(L, -1) ) // Check if we got a valid index
			{
				// get Index
				int _index = lua_tonumber(L, -1);
				
				T** obj = static_cast<T**>( lua_touserdata( L , 1 ) );
				
				if( _index & ( 1 << 8 ) ) // A func
				{
					lua_pushnumber(L, _index ^ ( 1 << 8 ) ); 	// Push the right func index
					lua_pushlightuserdata(L, obj); 				// and the corresponding object onto the stack
					lua_pushcclosure(L, &Lunar < T >::function_dispatch, 2); // Push a C Closure having the index and the object as upvalues
					return 1; // Return a func
				}
				
				lua_pop(L,2);    // Pop metatable and _index
				lua_remove(L,1); // Remove userdata
				lua_remove(L,1); // Remove [key]
				
				return ((*obj)->*(T::properties[_index].getter)) (L);
			}
			
			luaL_error( L , "Object has no such attribute" );
			
			return 0;
		}
		
		/*
			@ property_setter (internal)
			Arguments:
			* L - Lua State
		*/
		static int property_setter(lua_State* L)
		{
			lua_getmetatable(L, 1); // Look up the index from name
			lua_pushvalue(L, 2);	//
			lua_rawget(L, -2);		//
			
			if ( lua_isnumber(L, -1) ) // Check if we got a valid index
			{
				int _index = lua_tonumber(L, -1);
				
				T** obj = static_cast<T**>( lua_touserdata( L , 1 ) );
				
				if( !obj || !*obj ){
					_luafunc::errorHandler(L, "Internal error, no object given!" );
					return 0;
				}
				
				if( _index >> 8 ){ // Try to set a func
					_luafunc::errorHandler(L, "Trying to set the method [%s] of class [%s]", (*obj)->T::methods[_index ^ ( 1 << 8 ) ].name , T::className );
					return 0;
				}
				
				lua_pop(L,2);    // Pop metatable and _index
				lua_remove(L,1); // Remove userdata
				lua_remove(L,1); // Remove [key]
				
				return ((*obj)->*(T::properties[_index].setter)) (L);
			}
			
			return 0;
		}
		
		/*
			@ function_dispatch (internal)
			Arguments:
			* L - Lua State
		*/
		static int function_dispatch(lua_State* L)
		{
			// Fetch function index
			int i = (int) lua_tonumber( L , lua_upvalueindex(1) );
			
			// Fetch object
			T** obj = static_cast <T**>( lua_touserdata( L , lua_upvalueindex(2) ) );
			
			return ((*obj)->*(T::methods[i].func)) (L);
		}
		
		/*
			@ function_dump (internal)
			Arguments:
			* L - Lua State
		*/
		static int function_dump(lua_State * L)
		{
			string output = "Dump of ";
			output += T::className;
			output += ":\nMethods:";
			
			for (int i = 0; T::methods[i].name; i++)
			{
				output += "\n - ";
				output += T::methods[i].name;
			}
			
			output += "\nProperties:";
			
			for (int i = 0; T::properties[i].name; i++)
			{
				output += "\n - ";
				output += T::properties[i].name;
			}
			
			_luafunc::errorHandler(L, output.c_str());
			
			return 0;
		}
		
		/*
			@ gc_obj (internal)
			Arguments:
			* L - Lua State
		*/
		static int gc_obj(lua_State * L)
		{
			T** obj = static_cast<T**>( lua_touserdata( L , -1 ) );
			
			if( obj && *obj )
				delete(*obj);
			
			return 0;
		}
		
		/*
			@ to_string (internal)
			Arguments:
			* L - Lua State
		*/
		static int to_string(lua_State* L)
		{
			void** obj = static_cast<void**>(lua_touserdata(L, -1));
			
			if( obj )
				lua_pushfstring( L , "%s (%p)" , T::className , (void*)( *obj ) );
			else
				lua_pushliteral( L , "Empty object" );
			
			return 1;
		}
};

#define LUA_CLASS_FUNC( class , name ) { #name , &class::name }
#define LUA_CLASS_FUNC_END { 0 , 0 }
#define LUA_CLASS_ATTR_END { 0 , 0 , 0 }

#endif