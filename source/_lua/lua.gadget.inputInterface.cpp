#include "_lua/lua.class.inputInterface.h"

/*##################################
##       Lua-Input-Interface      ##
##################################*/

_lua_interface_input::_lua_interface_input( _interface_input* i ) : 
	input( i )
{ }

//! setAlign
int _lua_interface_input::setAlign( lua_State* L ){ ((_interface_input*)this->input)->setAlign( string2align[ luaL_checkstring( L , 1 ) ] ); return 0; };

//! setVAlign
int _lua_interface_input::setVAlign( lua_State* L ){((_interface_input*)this->input)->setVAlign( string2valign[ luaL_checkstring( L , 1 ) ] ); return 0; };

//! getAlign
int _lua_interface_input::getAlign( lua_State* L ) { lua_pushstring( L , align2string[ ((_interface_input*)this->input)->getAlign() ].c_str() ); return 1; }

//! getVAlign
int _lua_interface_input::getVAlign( lua_State* L ) { lua_pushstring( L , valign2string[ ((_interface_input*)this->input)->getVAlign() ].c_str() ); return 1; }

//! setIntValue
int _lua_interface_input::setIntValue( lua_State* L ){ ((_interface_input*)this->input)->setIntValue( luaL_checkint( L , 1 ) ); return 0; };

//! setStrValue
int _lua_interface_input::setStrValue( lua_State* L ){ ((_interface_input*)this->input)->setStrValue( luaL_checkstring( L , 1 ) ); return 0; };

//! setTitle
int _lua_interface_input::setTitle( lua_State* L ){ ((_interface_input*)this->input)->setTitle( luaL_checkstring( L , 1 ) ); return 0; };

//! getIntValue
int _lua_interface_input::getIntValue( lua_State* L ){ lua_pushnumber( L , ((_interface_input*)this->input)->getIntValue() ); return 1; }

//! getStrValue
int _lua_interface_input::getStrValue( lua_State* L ){ lua_pushstring( L , ((_interface_input*)this->input)->getStrValue().c_str() ); return 1; }

//! getTitle
int _lua_interface_input::getTitle( lua_State* L ){ lua_pushstring( L , ((_interface_input*)this->input)->getTitle().c_str() ); return 1; }