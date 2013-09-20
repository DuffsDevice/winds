#include "_lua/lua.gadget.counter.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

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
			this->setGadget( new _counter( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) , lightcheck<bool>( L , 4 , false ) , lightcheck<int>( L , 5 , 0 ) , lightcheck<_style>( L , 6 , _style() ) ) );
			break;
		case 6:
			//	_counter( _coord x , _coord y , _length width , bool circular , _s32 value , _s32 upperBound , _s32 lowerBound = 0 , _style style = _style() );
			this->setGadget( new _counter( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) , check<bool>( L , 4 ) , check<int>( L , 5 ) , check<int>( L , 6 ) , lightcheck<int>( L , 7 , 0 ) , lightcheck<int>( L , 8 , 0 ) , lightcheck<_style>( L , 9 , _style() ) ) );
			break;
		default:
			check<int>( L , 1 ); // Throw Reasonable error
			check<int>( L , 2 );
			check<int>( L , 3 );
			break;
	}
	
	_lua_interface_input::input = (_counter*)_lua_gadget::getGadget();
}

//! setUpperBound
int _lua_counter::setUpperBound( lua_State* L ){ _lua_interface_input::input->setUpperBound( check<int>( L , 1 ) ); return 0; }

//! setLowerBound
int _lua_counter::setLowerBound( lua_State* L ){ _lua_interface_input::input->setLowerBound( check<int>( L , 1 ) ); return 0; }

//! getLowerBound
int _lua_counter::getLowerBound( lua_State* L ){ lua_pushnumber( L , _lua_interface_input::input->getUpperBound() ); return 1; }

//! getUpperBound
int _lua_counter::getUpperBound( lua_State* L ){ lua_pushnumber( L , _lua_interface_input::input->getLowerBound() ); return 1; }

//! increase
int _lua_counter::increase( lua_State* L ){ _lua_interface_input::input->increase(); return 0; }

//! decrease
int _lua_counter::decrease( lua_State* L ){ _lua_interface_input::input->decrease(); return 0; }

//! Lua-button
const char _lua_counter::className[] = "Counter";
Lunar<_lua_counter>::FunctionType _lua_counter::methods[] = {
	LUA_CLASS_FUNC(_lua_counter,increase),
	LUA_CLASS_FUNC(_lua_counter,decrease),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_counter>::PropertyType _lua_counter::properties[] = {
	GADGET_BASE_ATTR,
	{ "value" , &_lua_counter::getIntValue , &_lua_counter::setIntValue },
	{ "upperBound" , &_lua_counter::getUpperBound , &_lua_counter::setUpperBound },
	{ "lowerBound" , &_lua_counter::getLowerBound , &_lua_counter::setLowerBound },
	LUA_CLASS_ATTR_END
};