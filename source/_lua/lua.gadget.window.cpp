#include "_lua/lua.gadget.window.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##           Lua-Window           ##
##################################*/

_lua_window::_lua_window( lua_State* L ) : 
	_lua_gadget()
	, _lua_interface_input()
{
	_lua_bitmap* bmp = Lunar<_lua_bitmap>::lightcheck( L , 6 );
	
	if( bmp && bmp->bm )
	{
		this->setGadget( 
			new _window(
				check<int>( L , 1 )
				, check<int>( L , 2 )
				, check<int>( L , 3 )
				, check<int>( L , 4 )
				, check<string>( L , 5 )
				, *bmp->bm
				, lightcheck<bool>( L , 7 , true )
				, lightcheck<bool>( L , 8 , true )
				, lightcheck<_style>( L , 9 , _style() | _styleAttr::draggable )
			) 
		);
	}
	else
	{
		this->setGadget(
			new _window(
				check<int>( L , 1 )
				, check<int>( L , 2 )
				, check<int>( L , 3 )
				, check<int>( L , 4 )
				, check<string>( L , 5 )
				, lightcheck<bool>( L , 6 , true )
				, lightcheck<bool>( L , 7 , true )
				, lightcheck<_style>( L , 8 , _style() | _styleAttr::draggable ) 
			)
		);
	}
	
	_lua_interface_input::input = (_window*)_lua_gadget::getGadget();
}

//! setIcon
int _lua_window::setIcon( lua_State* L )
{
	_lua_bitmap* bmp = Lunar<_lua_bitmap>::check( L , 1 ); 
	if( !bmp )
		return 0;
	_lua_interface_input::input->setIcon( *(bmp->bm) );
	return 0;
}

//! hasIcon
int _lua_window::hasIcon( lua_State* L )
{
	lua_pushnumber( L , _lua_interface_input::input->hasIcon() );
	return 1;
}

//! getIcon
int _lua_window::getIcon( lua_State* L )
{
	Lunar<_lua_bitmap>::push( L , new _lua_bitmap( &_lua_interface_input::input->getIcon() ) );
	return 1;
}

//! isMinimizeable
int _lua_window::isMinimizeable( lua_State* L )
{
	lua_pushboolean( L , _lua_interface_input::input->isMinimizeable() );
	return 1;
}

//! setMinimizeable
int _lua_window::setMinimizeable( lua_State* L )
{
	_lua_interface_input::input->setMinimizeable( check<bool>( L , 1 ) );
	return 0;
}

//! isMinimized
int _lua_window::isMinimized( lua_State* L )
{
	lua_pushboolean( L , _lua_interface_input::input->isMinimized() );
	return 1;
}

//! isMaximized
int _lua_window::isMaximized( lua_State* L )
{
	lua_pushboolean( L , _lua_interface_input::input->isMaximized() );
	return 1;
}

//! setCloseable
int _lua_window::setCloseable( lua_State* L )
{
	_lua_interface_input::input->setCloseable( check<bool>( L , 1 ) );
	return 0;
}

//! isCloseable
int _lua_window::isCloseable( lua_State* L )
{
	lua_pushboolean( L , _lua_interface_input::input->isCloseable() );
	return 1;
}

//! maximize
int _lua_window::maximize( lua_State* L )
{
	_lua_interface_input::input->maximize();
	return 0;
}

//! unMaximize
int _lua_window::unMaximize( lua_State* L )
{
	_lua_interface_input::input->unMaximize();
	return 0;
}

//! minimize
int _lua_window::minimize( lua_State* L )
{
	_lua_interface_input::input->minimize();
	return 0;
}

//! restore
int _lua_window::restore( lua_State* L )
{
	_lua_interface_input::input->restore();
	return 0;
}

//! Lua-window
const char _lua_window::className[] = "Window";
Lunar<_lua_window>::FunctionType _lua_window::methods[] = {
	LUA_CLASS_FUNC( _lua_window , hasIcon ),
	LUA_CLASS_FUNC( _lua_window , isMinimized ),
	LUA_CLASS_FUNC( _lua_window , isMaximized ),
	LUA_CLASS_FUNC( _lua_window , maximize ),
	LUA_CLASS_FUNC( _lua_window , unMaximize ),
	LUA_CLASS_FUNC( _lua_window , minimize ),
	LUA_CLASS_FUNC( _lua_window , restore ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_window>::PropertyType _lua_window::properties[] = {
	GADGET_BASE_ATTR,
	{ "title" , &_lua_window::getStrValue , &_lua_window::setStrValue },
	{ "icon" , &_lua_window::getIcon , &_lua_window::setIcon },
	{ "minimizeable" , &_lua_window::isMinimizeable , &_lua_window::setMinimizeable },
	LUA_CLASS_ATTR_END
};