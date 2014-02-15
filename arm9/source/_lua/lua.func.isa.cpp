#include "_lua/lua.func.h"
#include "_lua/lua.class.event.h"
#include "_lua/lua.class.rect.h"
#include "_lua/lua.class.area.h"
#include "_lua/lua.class.font.h"
#include "_lua/lua.class.border.h"
#include "_lua/lua.class.color.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.class.bitmap.port.h"
#include "_lua/lua.class.time.h"
#include "_lua/lua.class.mime.h"
#include "_lua/lua.class.hardwarekeypattern.h"

namespace _luafunc
{
	namespace detail
	{
		bool is_a( lua_State* L , int index , _bitmapPort* dummy ){
			return _luafunc::is_a( L , index , _lua_bitmapport::className );
		}
		
		bool is_a( lua_State* L , int index , _hardwareKeyPattern* dummy ){
			return _luafunc::is_a( L , index , _lua_hardwarekeypattern::className );
		}
		
		bool is_a( lua_State* L , int index , _bitmap* dummy ){
			return _luafunc::is_a( L , index , _lua_bitmap::className );
		}
		
		bool is_a( lua_State* L , int index , _rect* dummy ){
			return _luafunc::is_a( L , index , _lua_rect::className );
		}
		
		bool is_a( lua_State* L , int index , _area* dummy ){
			return _luafunc::is_a( L , index , _lua_area::className );
		}
		
		bool is_a( lua_State* L , int index , _event* dummy ){
			return _luafunc::is_a( L , index , _lua_event::className );
		}
		
		bool is_a( lua_State* L , int index , const _font** dummy ){
			return _luafunc::is_a( L , index , _lua_font::className );
		}
		
		bool is_a( lua_State* L , int index , _border* dummy ){
			return _luafunc::is_a( L , index , _lua_border::className );
		}
		
		bool is_a( lua_State* L , int index , _time* dummy ){
			return _luafunc::is_a( L , index , _lua_time::className );
		}
		
		bool is_a( lua_State* L , int index , _color* dummy ){
			int type = get_type( L , index );
			return
				type == LUA_TSTRING
				|| type == LUA_TNUMBER
				|| (
					type == LUA_TUSERDATA
					&& _luafunc::is_a( L , index , _lua_color::className )
				);
		}
		
		bool is_a( lua_State* L , int index , _mimeType* dummy ){
			int type = get_type( L , index );
			return
				type == LUA_TSTRING
				|| type == LUA_TNUMBER
				|| (
					type == LUA_TUSERDATA
					&& _luafunc::is_a( L , index , _lua_mimetype::className )
				);
		}
	}
}