#ifndef _WIN_C_DEBUG_
#define _WIN_C_DEBUG_

#include "_type/type.controller.h"
#include "_type/type.uniqueptr.h"
#include "_type/type.direntry.h"

struct lua_State;
class _runtimeErrorDialog;

class _debugController : public _controller
{
	private:
		
		static _map<lua_State*,_uniquePtr<_runtimeErrorDialog>>		luaErrorDialogs;
		static _uniquePtr<_direntry>								debugFile;
		
	public:
		
		//! Inits the debug system
		static bool init();
		
		//! Shuts down the debug system
		static void end();
		
		//! Call this function to indicate a runtime-error that happened inside a lua-function
		static void luaErrorHandler( lua_State* state , _literal messageFormat , ... );
		
		//! Outputs the supplied message na dthe current date to the global log-file
		static void debug( _literal fmt , ... ) __attribute__(( format(gnu_printf, 1 , 2) ));
		static void vdebug( _literal fmt , va_list );
		
		//! Wait for the press of the Button A
		static void submit();
};

#endif