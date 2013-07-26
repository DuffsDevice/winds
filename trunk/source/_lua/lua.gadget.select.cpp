#include "_lua/lua.gadget.select.h"

/*##################################
##          Lua-Selectbox         ##
##################################*/

_lua_select::_lua_select( lua_State* L ) : 
	_lua_gadget( new _select( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , { /* Empty List */ } , luaL_optstyle( L , lua_istable( L , 5 ) ? 6 : 5 ) ) )
	, _lua_interface_input( (_select*)_lua_gadget::getGadget() )
{
	// Pass Table
	if( lua_istable( L , 5 ) )
	{
		// Receive list and clear it
		_contextMenuEntryList& list = _lua_interface_input::input->getList();
		list.clear();
		
		int i = 0;
		while( true )
		{
			lua_rawgeti( L , 1 , ++i );
			
			if( lua_isnil( L , -1 ) )
				break;
				
			list[ i - 1 ] = luaL_checkstring( L , -1 );
			
			// Pop Value
			lua_pop( L , 1 );
		}
		
		_lua_interface_input::input->refreshList();
	}
}

//! addIndex
int _lua_select::addIndex( lua_State* L ){
	_lua_interface_input::input->getList()[ luaL_checkint( L , 1 ) ] = luaL_checkstring( L , 2 );
	_lua_interface_input::input->refreshList();
	return 0;
}

//! removeIndex
int _lua_select::removeIndex( lua_State* L ){
	_lua_interface_input::input->getList().erase( luaL_checkint( L , 1 ) );
	_lua_interface_input::input->refreshList();
	return 0;
}

//! clearList
int _lua_select::clearList( lua_State* L ){
	_lua_interface_input::input->getList().clear();
	_lua_interface_input::input->refreshList();
	return 0;
}

//! getList
int _lua_select::getList( lua_State* L )
{
	_contextMenuEntryList& list = _lua_interface_input::input->getList();
	
	lua_createtable( L , list.size() , 0 );
	
	int i = 0;
	
	for( const _pair<_s32,string>& data : list )
	{
		lua_createtable( L , 2 , 0 ); // Create table for [1] = key and [2] = value
		
		lua_pushnumber( L , data.first );
		lua_rawseti( L , -2 , 1 ); // Push Key
		
		lua_pushstring( L , data.second.c_str() );
		lua_rawseti( L , -2 , 2 ); // Push Value
		
		lua_rawseti( L , -2 , ++i ); // Add to table
	}
	
	return 1;
}

//! setList
int _lua_select::setList( lua_State* L )
{
	// Receive list and clear it
	_contextMenuEntryList& list = _lua_interface_input::input->getList();
	list.clear();
	
	int i = 0;
	
	while( true )
	{
		lua_rawgeti( L , 1 , ++i );
		
		if( lua_isnil( L , -1 ) )
			break;
			
		list[ i - 1 ] = luaL_checkstring( L , -1 );
		
		lua_pop( L , 1 );
	}
	
	_lua_interface_input::input->refreshList();
	
	return 1;
}

//! getEntryFromNumber
int _lua_select::getEntryFromNumber( lua_State* L ){
	lua_pushstring( L , _lua_interface_input::input->getList()[luaL_checkint(L,1)].c_str() );
	return 1;
}

//! Lua-button
const char _lua_select::className[] = "_select";
Lunar<_lua_select>::FunctionType _lua_select::methods[] = {
	GADGET_FUNCS( _lua_select ),
	LUA_CLASS_FUNC(_lua_select,addIndex),
	LUA_CLASS_FUNC(_lua_select,removeIndex),
	LUA_CLASS_FUNC(_lua_select,clearList),
	LUA_CLASS_FUNC(_lua_select,getList),
	LUA_CLASS_FUNC(_lua_select,setList),
	LUA_CLASS_FUNC(_lua_select,getEntryFromNumber),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_select>::PropertyType _lua_select::properties[] = {
	GADGET_ATTRS( _lua_select ),
	{ "strValue" , &_lua_select::getStrValue , nullptr },
	{ "value" , &_lua_select::getIntValue , &_lua_select::setIntValue },
	LUA_CLASS_ATTR_END
};