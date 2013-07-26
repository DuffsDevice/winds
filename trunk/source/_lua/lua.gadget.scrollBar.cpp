#include "_lua/lua.gadget.scrollBar.h"

/*##################################
##         Lua-ScrollArea         ##
##################################*/

_lua_scrollbar::_lua_scrollbar( lua_State* L ) :
	_lua_gadget(
		// _scrollBar( _coord x , _coord y , _u32 gadgetLength , _u32 length , _u32 length2 , _dimension dim , _u32 value = 0 , _style style = _style() );
		new _scrollBar(
			luaL_checkint( L , 1 ) // x
			, luaL_checkint( L , 2 ) // y
			, luaL_checkint( L , 3 ) // gadgetLength
			, luaL_checkint( L , 4 ) // length
			, luaL_checkint( L , 5 ) // length2
			, string2dimension[ luaL_checkstring( L , 4 ) ] // dim
			, luaL_optint( L , 7 , 0 ) // // value
			, luaL_optstyle( L , 8 )
		)
	)
	, _lua_interface_input( (_scrollBar*)_lua_gadget::getGadget() )
{ }

//! setValue
int _lua_scrollbar::setValue( lua_State* L ){ _lua_interface_input::input->setValue( luaL_checkint( L , 1 ) ); return 0; };

//! getValue
int _lua_scrollbar::getValue( lua_State* L ){ lua_pushnumber( L , _lua_interface_input::input->getValue() ); return 0; }

//! setDimension
int _lua_scrollbar::setDimension( lua_State* L ){ _lua_interface_input::input->setDimension( string2dimension[ luaL_checkstring( L , 1 ) ] ); return 0; };

//! getDimension
int _lua_scrollbar::getDimension( lua_State* L ){ lua_pushstring( L , dimension2string[ _lua_interface_input::input->getDimension() ].c_str() ); return 1; }

//! setStep
int _lua_scrollbar::setStep( lua_State* L ){ _lua_interface_input::input->setStep( luaL_checkint( L , 1 ) ); return 0; };

//! getStep
int _lua_scrollbar::getStep( lua_State* L ){ lua_pushnumber( L , _lua_interface_input::input->getStep() ); return 0; }

//! setLength
int _lua_scrollbar::setLength( lua_State* L ){ _lua_interface_input::input->setLength( luaL_checkint( L , 1 ) ); return 0; };

//! getLength
int _lua_scrollbar::getLength( lua_State* L ){ lua_pushnumber( L , _lua_interface_input::input->getLength() ); return 0; }

//! setLength2
int _lua_scrollbar::setLength2( lua_State* L ){ _lua_interface_input::input->setLength2( luaL_checkint( L , 1 ) ); return 0; };

//! getLength2
int _lua_scrollbar::getLength2( lua_State* L ){ lua_pushnumber( L , _lua_interface_input::input->getLength2() ); return 0; }

//! Lua-button
const char _lua_scrollbar::className[] = "_scrollbar";
Lunar<_lua_scrollbar>::FunctionType _lua_scrollbar::methods[] = {
	GADGET_FUNCS( _lua_scrollbar ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_scrollbar>::PropertyType _lua_scrollbar::properties[] = {
	GADGET_ATTRS( _lua_scrollbar ),
	{ "step" , &_lua_scrollbar::getStep , &_lua_scrollbar::setStep },
	{ "length" , &_lua_scrollbar::getLength , &_lua_scrollbar::setLength },
	{ "length2" , &_lua_scrollbar::getLength2 , &_lua_scrollbar::setLength2 },
	{ "value" , &_lua_scrollbar::getValue , &_lua_scrollbar::setValue },
	{ "dimension" , &_lua_scrollbar::getDimension , &_lua_scrollbar::setDimension },
	LUA_CLASS_ATTR_END
};