#include "_lua/lua.gadget.colorpicker.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##         Lua-Colorpicker        ##
##################################*/

_lua_colorpicker::_lua_colorpicker( lua_State* L )
{
	// _colorpicker( _length width , _length height , _coord x , _coord y , _pixel initialColor = COLOR_BLUE , _style style = _style() );
	this->setGadget( new _colorpicker( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) , check<int>( L , 4 ) , lightcheck<_pixel>( L , 5 , COLOR_BLUE ) , lightcheck<_style>( L , 6 , _style() ) ) );
	
	_lua_interface_input::input = (_colorpicker*)_lua_gadget::getGadget();
}

//! Lua-button
const char _lua_colorpicker::className[] = "ColorPicker";
Lunar<_lua_colorpicker>::FunctionType _lua_colorpicker::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_colorpicker>::PropertyType _lua_colorpicker::properties[] = {
	GADGET_BASE_ATTR,
	{ "color" , &_lua_colorpicker::getColor , &_lua_colorpicker::setColor },
	LUA_CLASS_ATTR_END
};