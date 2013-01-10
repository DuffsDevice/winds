#include "_lua/lua.gadget.progressbar.h"

/*##################################
##         Lua-Progressbar        ##
##################################*/

_lua_progressbar::_lua_progressbar( lua_State* L )
{
	switch( lua_gettop( L ) )
	{
		case 3:	
			//	_progressbar( _length width , _coord x , _coord y  , bool type = true , _style style = _style() );
			this->setGadget( new _progressbar( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) ) );
			break;
		case 4:
			if( !lua_isboolean( L , 4 ) )
			{
				_length l1 = luaL_checkint( L , 1 );
				_length l2 = luaL_checkint( L , 2 );
				_coord c1 = luaL_checkint( L , 3 );
				_coord c2 = luaL_checkint( L , 4 );
				//	_progressbar( _length width , _coord x , _coord y  , bool type = true , _style style = _style() );
				this->setGadget( new _progressbar( l1 , l2 , c1 , c2 ) );
			}
			else
				//	_progressbar( _length width , _length height , _coord x , _coord y  , bool type = true , _style style = _style() );
				this->setGadget( new _progressbar( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkboolean( L , 4 ) ) );
			break;
		case 5:
			//	_progressbar( _length width , _length height , _coord x , _coord y  , bool type = true , _style style = _style() );
			this->setGadget( new _progressbar( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkboolean( L , 5 ) ) );
			break;
		default:
			luaL_checkint( L , 1 ); // Print reasonable error-message
			luaL_checkint( L , 2 );
			luaL_checkint( L , 3 );
			break;
	}
	
	_lua_interface_input::input = (_progressbar*)_lua_gadget::gadget;
}

//! setColorScheme
int _lua_progressbar::setColorScheme( lua_State* L ){ ((_progressbar*)_lua_gadget::gadget)->setColorScheme( luaL_checkboolean( L , 1 ) ); return 0; };

//! getColorScheme
int _lua_progressbar::getColorScheme( lua_State* L ){ lua_pushboolean( L , ((_progressbar*)_lua_gadget::gadget)->getColorScheme() ); return 1; }

//! setBarType
int _lua_progressbar::setBarType( lua_State* L ){	((_progressbar*)_lua_gadget::gadget)->setBarType( luaL_checkboolean( L , 1 ) ); return 0; };

//! getBarType
int _lua_progressbar::getBarType( lua_State* L ){ lua_pushboolean( L , ((_progressbar*)_lua_gadget::gadget)->getColorScheme() ); return 1; }

//! Lua-button
const char _lua_progressbar::className[] = "_progressbar";
Lunar<_lua_progressbar>::FunctionType _lua_progressbar::methods[] = {
	GADGET_FUNCS( _lua_progressbar ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_progressbar>::PropertyType _lua_progressbar::properties[] = {
	GADGET_ATTRS( _lua_progressbar ),
	{ "intValue" , &_lua_progressbar::getIntValue , &_lua_progressbar::setIntValue },
	{ "colorScheme" , &_lua_progressbar::getColorScheme , &_lua_progressbar::setColorScheme },
	{ "barType" , &_lua_progressbar::getBarType , &_lua_progressbar::setBarType },
	LUA_CLASS_ATTR_END
};