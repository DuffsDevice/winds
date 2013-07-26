#include "_lua/lua.gadget.colorpicker.h"

/*##################################
##         Lua-Colorpicker        ##
##################################*/

_lua_colorpicker::_lua_colorpicker( lua_State* L )
{
	// _colorpicker( _length width , _length height , _coord x , _coord y , _pixel initialColor = COLOR_BLUE , _style style = _style() );
	this->setGadget( new _colorpicker( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_optcolor( L , 5 , COLOR_BLUE ) , luaL_optstyle( L , 6 ) ) );
	
	_lua_interface_input::input = (_colorpicker*)_lua_gadget::getGadget();
}

//! Lua-button
const char _lua_colorpicker::className[] = "_colorpicker";
Lunar<_lua_colorpicker>::FunctionType _lua_colorpicker::methods[] = {
	GADGET_FUNCS( _lua_colorpicker ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_colorpicker>::PropertyType _lua_colorpicker::properties[] = {
	GADGET_ATTRS( _lua_colorpicker ),
	{ "color" , &_lua_colorpicker::getColor , &_lua_colorpicker::setColor },
	LUA_CLASS_ATTR_END
};