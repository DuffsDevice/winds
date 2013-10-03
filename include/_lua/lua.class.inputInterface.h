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
		
		//! A way to get a pointer to the derived instance of _lua_gadget
		T* getDerived(){ return (T*)this; }
		
		// Virtual Dtor
		virtual ~_lua_interface_input(){};
		
		//! setAlign
		int setAlign( lua_State* L ){ getDerived()->setAlign( string2align[ check<string>( L , -1 ) ] ); return 0; };
		
		//! setVAlign
		int setVAlign( lua_State* L ){ getDerived()->setVAlign( string2valign[ check<string>( L , -1 ) ] ); return 0; };
		
		//! getAlign
		int getAlign( lua_State* L ) { push( L , align2string[ getDerived()->getAlign() ].c_str() ); return 1; }
		
		//! getVAlign
		int getVAlign( lua_State* L ) { push( L , valign2string[ getDerived()->getVAlign() ].c_str() ); return 1; }
		
		//! setIntValue
		int setIntValue( lua_State* L ){ getDerived()->setIntValue( check<int>( L , -1 ) ); return 0; };
		
		//! setStrValue
		int setStrValue( lua_State* L ){ getDerived()->setStrValue( check<string>( L , -1 ) ); return 0; };
		
		//! setTitle
		int setTitle( lua_State* L ){ getDerived()->setTitle( check<string>( L , -1 ) ); return 0; };
		
		//! getIntValue
		int getIntValue( lua_State* L ){ push( L , getDerived()->getIntValue() ); return 1; }
		
		//! getStrValue
		int getStrValue( lua_State* L ){ push( L , getDerived()->getStrValue().c_str() ); return 1; }
		
		//! getTitle
		int getTitle( lua_State* L ){ push( L , getDerived()->getTitle().c_str() ); return 1; }
		
		//! getColor
		int getColor( lua_State* L ){ push( L , getDerived()->getColor() ); return 1; }
		
		//! setColor
		int setColor( lua_State* L ){ getDerived()->setColor( check<_pixel>( L , 1 ) ); return 0; }
		
		//! setBgColor
		int setBgColor( lua_State* L ){ getDerived()->setBgColor( check<_pixel>( L , 1 ) ); return 0; }
		
		//! getBgColor
		int getBgColor( lua_State* L ){ push( L , getDerived()->getBgColor() ); return 1; }
		
		//! setFont
		int setFont( lua_State* L ){
			_lua_font* f = Lunar<_lua_font>::check( L , 1 );
			if( f && f->font )
				getDerived()->setFont( f->font );
			return 0;
		}
		
		//! getFont
		int getFont( lua_State* L ){
			Lunar<_lua_font>::push( L , new _lua_font( getDerived()->getFont() ) ); return 1;
			return 1;
		}
		
		//! setFontSize
		int setFontSize( lua_State* L ){ getDerived()->setFontSize( check<int>( L , 1 ) ); return 0; }
		
		//! getFontSize
		int getFontSize( lua_State* L ){ push( L , getDerived()->getFontSize() ); return 1; }
};

#endif