#include "_lua/lua.func.h"
#include "_lua/lua.class.event.h"
#include "_lua/lua.class.rect.h"
#include "_lua/lua.class.area.h"
#include "_lua/lua.class.font.h"
#include "_lua/lua.class.border.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.class.bitmap.port.h"

namespace _luafunc
{
	namespace detail
	{
		bool is_a( lua_State* L , int index , _bitmapPort* dummy ){
			return _luafunc::is_a( L , index , _lua_bitmapPort::className );
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
	}
}