#include <_lua/lua.func.h>
#include <_lua/lua.lunar.h>
#include <_lua/lua.class.gadget.h>
#include <_lua/lua.class.event.h>
#include <_lua/lua.class.rect.h>
#include <_lua/lua.class.area.h>
#include <_lua/lua.class.font.h>
#include <_lua/lua.class.color.h>
#include <_lua/lua.class.border.h>
#include <_lua/lua.class.mime.h>
#include <_lua/lua.class.menu.h>
#include <_lua/lua.class.bitmap.h>
#include <_lua/lua.class.bitmap.port.h>
#include <_lua/lua.class.hardwarekeypattern.h>
#include <_lua/lua.class.time.h>

namespace _luafunc
{
	namespace detail
	{
		_gadget* check( lua_State* L , int index , _gadget** dummy )
		{
			_lua_gadget* gadget = Lunar<_lua_gadget>::check( L , index );
			return gadget ? gadget->getGadget() : nullptr;
		}
		
		_bitmap check( lua_State* L , int index , _bitmap* dummy ){
			_lua_bitmap* bmp = Lunar<_lua_bitmap>::check( L , index );
			return bmp ? (_bitmap&)*bmp : _bitmap();
		}
		
		_border check( lua_State* L , int index , _border* dummy ){
			_lua_border* b = Lunar<_lua_border>::check( L , index );
			return b ? *b : _border();
		}
		
		_time check( lua_State* L , int index , _time* dummy ){
			_lua_time* b = Lunar<_lua_time>::check( L , index );
			return b ? *b : _time();
		}
		
		_menu check( lua_State* L , int index , _menu* dummy )
		{
			if( get_type( L , index ) == LUA_TTABLE )
				return _menu( check( L , index , (_menuEntryList*)nullptr ) );
			
			_lua_menu* m = Lunar<_lua_menu>::check( L , index );
			return m ? (_menu&)*m : _menu();
		}
		
		_color check( lua_State* L , int index , _color* dummy )
		{
			int type = get_type( L , index );
			
			if(	type == LUA_TSTRING )
				return string( lua_tostring( L , index ) );
			else if( type == LUA_TNUMBER )
				return _pixel( lua_tointeger( L , index ) );
			
			_lua_color* c = Lunar<_lua_color>::check( L , index );
			return c ? *c : _color();
		}
		
		_bitmapPort check( lua_State* L , int index , _bitmapPort* dummy )
		{
			static _bitmap bitmapFallback;
			
			_lua_bitmapport* bmp = Lunar<_lua_bitmapport>::check( L , index );
			if( bmp )
				return *bmp;
			
			return _bitmapPort(bitmapFallback);
		}
		
		_hardwareKeyPattern check( lua_State* L , int index , _hardwareKeyPattern* dummy ){
			_lua_hardwarekeypattern* hwkp = Lunar<_lua_hardwarekeypattern>::check( L , index );
			return hwkp ? *hwkp : _hardwareKeyPattern();
		}
		
		_rect check( lua_State* L , int index , _rect* dummy ){
			_lua_rect* rc = Lunar<_lua_rect>::check( L , index );
			return rc ? *rc : _rect();
		}
		
		_area check( lua_State* L , int index , _area* dummy ){
			_lua_area* ar = Lunar<_lua_area>::check( L , index );
			return ar ? *ar : _area();
		}
		
		_event check( lua_State* L , int index , _event* dummy ){
			_lua_event* ev = Lunar<_lua_event>::check( L , 1 );
			return ev ? *ev : _event();
		}
		
		_fontHandle check( lua_State* L , int index , _fontHandle* dummy ){
			_lua_font* ft = Lunar<_lua_font>::check( L , index );
			if( ft )
				return (_fontHandle)*ft; // You CAN cast _lua_font to _fontHandle
			return nullptr;
		}
		
		_mimeType check( lua_State* L , int index , _mimeType* dummy ){
			_lua_mimetype* mt = Lunar<_lua_mimetype>::check( L , index );
			if( mt )
				return *mt;
			return string( luaL_checkstring( L , index ) );
		}
	}
}