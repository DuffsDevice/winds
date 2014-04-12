#include "_lua/lua.func.h"
#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.event.h"
#include "_lua/lua.class.rect.h"
#include "_lua/lua.class.area.h"
#include "_lua/lua.class.font.h"
#include "_lua/lua.class.color.h"
#include "_lua/lua.class.border.h"
#include "_lua/lua.class.mime.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.class.bitmap.port.h"
#include "_lua/lua.class.hardwarekeypattern.h"
#include "_lua/lua.class.time.h"

namespace _luafunc
{
	namespace detail
	{
		_gadget* check( lua_State* L , int index , _gadget** dummy )
		{
			_lua_gadget* gadget = Lunar<_lua_gadget>::lightcheck( L , index );
			return gadget ? gadget->getGadget() : nullptr;
		}
		
		_bitmap check( lua_State* L , int index , _bitmap* dummy ){
			_lua_bitmap* bmp = Lunar<_lua_bitmap>::lightcheck( L , index );
			return bmp ? (_bitmap&)*bmp : _bitmap();
		}
		
		_border check( lua_State* L , int index , _border* dummy ){
			_lua_border* b = Lunar<_lua_border>::lightcheck( L , index );
			return b ? *b : _border();
		}
		
		_time check( lua_State* L , int index , _time* dummy ){
			_lua_time* b = Lunar<_lua_time>::lightcheck( L , index );
			return b ? *b : _time();
		}
		
		_color check( lua_State* L , int index , _color* dummy )
		{
			int type = get_type( L , index );
			
			if(	type == LUA_TSTRING )
				return string( lua_tostring( L , index ) );
			else if( type == LUA_TNUMBER )
				return _pixel( luaL_checkint( L , index ) );
			
			_lua_color* c = Lunar<_lua_color>::lightcheck( L , index );
			return c ? *c : _color();
		}
		
		_bitmapPort check( lua_State* L , int index , _bitmapPort* dummy )
		{
			static _bitmap bitmapFallback;
			
			_lua_bitmapport* bmp = Lunar<_lua_bitmapport>::lightcheck( L , index );
			if( bmp )
				return *bmp;
			
			return _bitmapPort(bitmapFallback);
		}
		
		_hardwareKeyPattern check( lua_State* L , int index , _hardwareKeyPattern* dummy ){
			_lua_hardwarekeypattern* hwkp = Lunar<_lua_hardwarekeypattern>::lightcheck( L , index );
			return hwkp ? *hwkp : _hardwareKeyPattern();
		}
		
		_rect check( lua_State* L , int index , _rect* dummy ){
			_lua_rect* rc = Lunar<_lua_rect>::lightcheck( L , index );
			return rc ? *rc : _rect();
		}
		
		_area check( lua_State* L , int index , _area* dummy ){
			_lua_area* ar = Lunar<_lua_area>::lightcheck( L , index );
			return ar ? *ar : _area();
		}
		
		_event check( lua_State* L , int index , _event* dummy ){
			_lua_event* ev = Lunar<_lua_event>::check( L , 1 );
			return ev ? *ev : _event();
		}
		
		_fontPtr check( lua_State* L , int index , _fontPtr* dummy ){
			_lua_font* ft = Lunar<_lua_font>::lightcheck( L , index );
			if( ft )
				return (_fontPtr)*ft; // You CAN cast _lua_font to _font*
			return nullptr;
		}
		
		_mimeType check( lua_State* L , int index , _mimeType* dummy ){
			_lua_mimetype* mt = Lunar<_lua_mimetype>::lightcheck( L , index );
			if( mt )
				return *mt;
			return string( luaL_checkstring( L , index ) );
		}
	}
}