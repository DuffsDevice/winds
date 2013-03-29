#include "_lua/lua.gadget.counter.h"

/*##################################
##           Lua-Counter          ##
##################################*/

_lua_counter::_lua_counter( lua_State* L )
{
	switch( lua_gettop( L ) )
	{
		case 3:
		case 4:
		case 5:
			//	_counter( _coord x , _coord y , _length width , bool circular = false , _s32 value = 0 , _style style = _style() );
			this->setGadget( new _counter( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_optboolean( L , 4 , false ) , luaL_optint( L , 5 , 0 ) , luaL_optstyle( L , 6 ) ) );
			break;
		case 6:
			//	_counter( _coord x , _coord y , _length width , bool circular , _s32 value , _s32 upperBound , _s32 lowerBound = 0 , _style style = _style() );
			this->setGadget( new _counter( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkboolean( L , 4 ) , luaL_checkint( L , 5 ) , luaL_checkint( L , 6 ) , luaL_optint( L , 7 , 0 ) , luaL_optint( L , 8 , 0 ) , luaL_optstyle( L , 9 ) ) );
			break;
		default:
			luaL_checkint( L , 1 ); // Throw Reasonable error
			luaL_checkint( L , 2 );
			luaL_checkint( L , 3 );
			break;
	}
	
	_lua_interface_input::input = (_counter*)_lua_gadget::gadget;
}

//! setUpperBound
int _lua_counter::setUpperBound( lua_State* L ){ _lua_interface_input::input->setUpperBound( luaL_checkint( L , 1 ) ); return 0; }

//! setLowerBound
int _lua_counter::setLowerBound( lua_State* L ){ _lua_interface_input::input->setLowerBound( luaL_checkint( L , 1 ) ); return 0; }

//! getLowerBound
int _lua_counter::getLowerBound( lua_State* L ){ lua_pushnumber( L , _lua_interface_input::input->getUpperBound() ); return 1; }

//! getUpperBound
int _lua_counter::getUpperBound( lua_State* L ){ lua_pushnumber( L , _lua_interface_input::input->getLowerBound() ); return 1; }

//! Lua-button
const char _lua_counter::className[] = "_counter";
Lunar<_lua_counter>::FunctionType _lua_counter::methods[] = {
	GADGET_FUNCS( _lua_counter ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_counter>::PropertyType _lua_counter::properties[] = {
	GADGET_ATTRS( _lua_counter ),
	{ "intValue" , &_lua_counter::getIntValue , &_lua_counter::setIntValue },
	{ "upperBound" , &_lua_counter::getUpperBound , &_lua_counter::setUpperBound },
	{ "lowerBound" , &_lua_counter::getLowerBound , &_lua_counter::setLowerBound },
	LUA_CLASS_ATTR_END
};