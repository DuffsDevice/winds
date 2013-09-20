// Check if already included
#ifndef _WIN_L_INPUTINTERFACE_
#define _WIN_L_INPUTINTERFACE_

#include "_lua/lunar.h"
#include "_lua/lua.class.font.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/**
 * Proxy Classes
 */
template<typename T>
class _lua_interface_input{
	
	public:
		
		T*	input;
		
		// Virtual Dtor
		virtual ~_lua_interface_input(){};
		
		// Ctor
		_lua_interface_input( T* i = nullptr ) : 
			input( i )
		{ }
		
		//! setAlign
		int setAlign( lua_State* L ){ this->input->setAlign( string2align[ luaL_checkstring( L , -1 ) ] ); return 0; };
		
		//! setVAlign
		int setVAlign( lua_State* L ){ this->input->setVAlign( string2valign[ luaL_checkstring( L , -1 ) ] ); return 0; };
		
		//! getAlign
		int getAlign( lua_State* L ) { lua_pushstring( L , align2string[ this->input->getAlign() ].c_str() ); return 1; }
		
		//! getVAlign
		int getVAlign( lua_State* L ) { lua_pushstring( L , valign2string[ this->input->getVAlign() ].c_str() ); return 1; }
		
		//! setIntValue
		int setIntValue( lua_State* L ){ this->input->setIntValue( luaL_checkint( L , -1 ) ); return 0; };
		
		//! setStrValue
		int setStrValue( lua_State* L ){ this->input->setStrValue( luaL_checkstring( L , -1 ) ); return 0; };
		
		//! setTitle
		int setTitle( lua_State* L ){ this->input->setTitle( luaL_checkstring( L , -1 ) ); return 0; };
		
		//! getIntValue
		int getIntValue( lua_State* L ){ lua_pushnumber( L , this->input->getIntValue() ); return 1; }
		
		//! getStrValue
		int getStrValue( lua_State* L ){ lua_pushstring( L , this->input->getStrValue().c_str() ); return 1; }
		
		//! getTitle
		int getTitle( lua_State* L ){ lua_pushstring( L , this->input->getTitle().c_str() ); return 1; }
		
		//! getColor
		int getColor( lua_State* L ){ lua_pushnumber( L , this->input->getColor() ); return 1; }
		
		//! setColor
		int setColor( lua_State* L ){ this->input->setColor( check<_pixel>( L , 1 ) ); return 0; }
		
		//! setBgColor
		int setBgColor( lua_State* L ){ this->input->setBgColor( check<_pixel>( L , 1 ) ); return 0; }
		
		//! getBgColor
		int getBgColor( lua_State* L ){ lua_pushnumber( L , this->input->getBgColor() ); return 1; }
		
		//! setFont
		int setFont( lua_State* L ){
			_lua_font* f = Lunar<_lua_font>::check( L , 1 );
			if( f && f->font )
				this->input->setFont( f->font );
			return 0;
		}
		
		//! getFont
		int getFont( lua_State* L ){
			Lunar<_lua_font>::push( L , new _lua_font( this->input->getFont() ) ); return 1;
			return 1;
		}
		
		//! setFontSize
		int setFontSize( lua_State* L ){ this->input->setFontSize( luaL_checkint( L , 1 ) ); return 0; }
		
		//! getFontSize
		int getFontSize( lua_State* L ){ lua_pushnumber( L , this->input->getFontSize() ); return 1; }
};

#endif