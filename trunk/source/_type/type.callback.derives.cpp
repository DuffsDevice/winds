#include "_type/type.callback.derives.h"
#include "_type/type.system.h"

//
// LUA
//
namespace luaCallbackFunctions
{
	void debug( const char* fmt , ... )
	{
		if( !fmt )
			return;
		
		// Declare a va_list type variable
		va_list args;
		
		// Initialise the va_list variable with the ... after fmt
		va_start(args, fmt);
		
		_system::vdebug( fmt , args );
		
		// Clean up the va_list
		va_end(args);
	}
}