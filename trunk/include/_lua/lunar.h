// Check if already included
#ifndef _WIN_LUNAR_
#define _WIN_LUNAR_

#include "_type/type.h"
#include "lua.hpp"

extern bool luaL_is( lua_State* L , int narg , string type );

template < class T > class Lunar {
    typedef struct {
	T              *pT;
    } userdataType;
    
  public:

    enum {
	NUMBER,
	STRING
    };

    struct PropertyType {
	const char     *name;
	int             (T::*getter) (lua_State *);
	int             (T::*setter) (lua_State *);
    };

    struct FunctionType {
	const char     *name;
	int             (T::*function) (lua_State *);
    };
	
	static void lua_gettablevalue (lua_State * state, int tableindex, int valueindex)
	{
		lua_pushnumber (state, valueindex);
		lua_gettable (state, tableindex);
		// use lua_to<type>(-1); to get the value.
	}

/*
  @ check
  Arguments:
    * L - Lua State
    * narg - Position to check

  Description:
    Retrieves a wrapped class from the arguments passed to the function, specified by narg (position).
    This function will raise an exception if the argument is not of the correct type.
*/
    static T       *check(lua_State * L, int narg) {
	// Check to see whether we are a table
	    if (lua_istable(L,narg+1))
	    {
		    lua_gettablevalue(L,narg+1,0);
		    userdataType   *ud =
				    static_cast <userdataType * >(luaL_checkudata(L, -1, T::className));
		    if (!ud)
			    luaL_argerror (L, narg+1, T::className);
		    lua_pop(L,1);
		    return ud->pT;		// pointer to T object
	    }
	    else
	    {
		    luaL_argerror (L, narg+1, T::className);
	    } 
		return nullptr;
    }

/*
  @ lightcheck
  Arguments:
    * L - Lua State
    * narg - Position to check

  Description:
    Retrieves a wrapped class from the arguments passed to the function, specified by narg (position).
    This function will return nullptr if the argument is not of the correct type.  Useful for supporting
    multiple types of arguments passed to the function
*/ 
	static T       *lightcheck(lua_State * L, int narg) {
		// Check to see whether we are a table
	    if (lua_istable(L,narg+1))
	    {
		    lua_gettablevalue(L,narg+1,0);
		    userdataType   *ud =
				    static_cast <userdataType * >(luaL_testudata(L, -1, T::className));
		    if (!ud)
			    return nullptr; // lightcheck returns nullptr if not found.
		    lua_pop(L,1);
		    return ud->pT;		// pointer to T object
	    }
	    else
	    {
		    return nullptr;
	    } 
    }

/*
  @ Register
  Arguments:
    * L - Lua State
    * namespac - Namespace to load into

  Description:
    Registers your class with Lua.  Leave namespac "" if you want to load it into the global space.
*/
    // REGISTER CLASS AS A GLOBAL TABLE 
    static void     Register(lua_State * L, string namespac = "" ) {

		if ( !namespac.length() ) {
			lua_getglobal(L, namespac.c_str() );
			lua_pushcfunction(L, &Lunar < T >::constructor);
			lua_setfield(L, -2, T::className);
			lua_pop(L, 1);
		} else {
			lua_pushcfunction(L, &Lunar < T >::constructor);
			lua_setglobal(L, T::className);
		}

		luaL_newmetatable(L, T::className);
		int             metatable = lua_gettop(L);

		lua_pushstring(L, "__gc");
		lua_pushcfunction(L, &Lunar < T >::gc_obj);
		lua_settable(L, metatable);

		lua_pushstring(L, "__index");
		lua_pushcfunction(L, &Lunar < T >::property_getter);
		lua_settable(L, metatable);

		lua_pushstring(L, "__newindex");
		lua_pushcfunction(L, &Lunar < T >::property_setter);
		lua_settable(L, metatable);

    }

/*
  @ constructor (internal)
  Arguments:
    * L - Lua State
*/
    static int      constructor(lua_State * L) {

		lua_newtable(L);

		int             newtable = lua_gettop(L);

		lua_pushnumber(L, 0);

		T             **a = (T **) lua_newuserdata(L, sizeof(T *));
		*a = new T(L);

		int             userdata = lua_gettop(L);

		luaL_getmetatable(L, T::className);

		lua_setmetatable(L, userdata);

		lua_settable(L, newtable);

		luaL_getmetatable(L, T::className);
		lua_setmetatable(L, newtable);

		luaL_getmetatable(L, T::className);

		for (int i = 0; T::properties[i].name; i++) {
			lua_pushstring(L, T::properties[i].name);
			lua_pushnumber(L, i);
			lua_settable(L, -3);
		}

		lua_pop(L, 1);

		for (int i = 0; T::methods[i].name; i++) {
			lua_pushstring(L, T::methods[i].name);
			lua_pushnumber(L, i);
			lua_pushcclosure(L, &Lunar < T >::function_dispatch, 1);
			lua_settable(L, newtable);
		}

		return 1;
    }

/*
  @ createNew
  Arguments:
    * L - Lua State
	T*	- Instance to push

  Description:
    Loads an instance of the class into the Lua stack, and provides you a pointer so you can modify it.
*/
    static void push(lua_State * L, T* instance ) {

		lua_newtable(L);

		int             newtable = lua_gettop(L);

		lua_pushnumber(L, 0);

		T             **a = (T **) lua_newuserdata(L, sizeof(T *));
		*a = instance;

		int             userdata = lua_gettop(L);

		luaL_getmetatable(L, T::className);

		lua_setmetatable(L, userdata);

		lua_settable(L, newtable);

		luaL_getmetatable(L, T::className);
		lua_setmetatable(L, newtable);

		luaL_getmetatable(L, T::className);

		for (int i = 0; T::properties[i].name; i++) {
			// ADD NAME KEY 
			lua_pushstring(L, T::properties[i].name);
			lua_pushnumber(L, i);
			lua_settable(L, -3);
		}

		lua_pop(L, 1);

		for (int i = 0; T::methods[i].name; i++) {
			lua_pushstring(L, T::methods[i].name);
			lua_pushnumber(L, i);
			lua_pushcclosure(L, &Lunar < T >::function_dispatch, 1);
			lua_settable(L, newtable);
		}

    }

/*
  @ property_getter (internal)
  Arguments:
    * L - Lua State
*/
    static int      property_getter(lua_State * L) {

		lua_pushvalue(L, 2);

		lua_getmetatable(L, 1);

		lua_pushvalue(L, 2);
		lua_rawget(L, -2);

		if (lua_isnumber(L, -1)) {

			int             _index = lua_tonumber(L, -1);

			lua_pushnumber(L, 0);
			lua_rawget(L, 1);

			T             **obj =
			static_cast < T ** >(lua_touserdata(L, -1));

			lua_pushvalue(L, 3);

			int (T::*getter) (lua_State *) = T::properties[_index].getter;

			if( getter )
				return ((*obj)->*(getter)) (L);
			else
				return 0;

		}
		// PUSH NIL 
		lua_pushnil(L);

		return 1;

    }

/*
  @ property_setter (internal)
  Arguments:
    * L - Lua State
*/
    static int      property_setter(lua_State * L) {
		
		lua_getmetatable(L, 1);

		lua_pushvalue(L, 2);
		lua_rawget(L, -2);

		if (lua_isnil(L, -1)) {

			lua_pop(L, 2);

			lua_rawset(L, 1);

			return 0;
		} else {

			int             _index = lua_tonumber(L, -1);

			lua_pushnumber(L, 0);
			lua_rawget(L, 1);

			T             **obj =
			static_cast < T ** >(lua_touserdata(L, -1));

			lua_pushvalue(L, 3);

			int (T::*setter) (lua_State *) = T::properties[_index].setter;

			if( setter )
				return ((*obj)->*(setter)) (L);
			else
				return 0;

		}

    }

/*
  @ function_dispatch (internal)
  Arguments:
    * L - Lua State
*/
    static int      function_dispatch(lua_State * L) {

		int             i = (int) lua_tonumber(L, lua_upvalueindex(1));

		lua_pushnumber(L, 0);
		lua_rawget(L, 1);

		T             **obj = static_cast < T ** >(lua_touserdata(L, -1));

		lua_pop(L, 1);

		return ((*obj)->*(T::methods[i].function)) (L);
    }

/*
  @ gc_obj (internal)
  Arguments:
    * L - Lua State
*/
    static int      gc_obj(lua_State * L) {

		T             **obj =
			static_cast < T ** >(lua_touserdata(L, -1));
		
		if ( *obj )
		{
			//printf("Cleaning up a %s.\n", T::className );
			delete(*obj);
		}

		return 0;
    }

};

#define LUA_CLASS_FUNC( class , name ) { #name , &class::name }
#define LUA_CLASS_ATTR( class , funcName , name ) { #name , &class::get##funcName , &class::set##funcName }
#define LUA_CLASS_ATTR_EX( class , getter , setter , name ) { #name , &class::getter , &class::setter }
#define LUA_CLASS_ATTR_RD( class , funcName , name ) { #name , &class::get##funcName , 0 }
#define LUA_CLASS_ATTR_WR( class , funcName , name ) { #name , 0 , &class::set##funcName }
#define LUA_CLASS_FUNC_END { 0 , 0 }
#define LUA_CLASS_ATTR_END { 0 , 0 , 0 }

#endif