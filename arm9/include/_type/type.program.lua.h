// Check if already included
#ifndef _WIN_T_LUAPROG_
#define _WIN_T_LUAPROG_

//! Lua
#include <_lua/lua.h>

//! Program
#include <_type/type.program.h>

class _progLua : public _program
{
	private:
		
		//! The lua Interpreter state
		lua_State* 				state;
		string*					content;
		bool					headParsed;
		bool					mainFrameAllocated;
		int						serviceDispatchTableIndex;
		
		//! Array filled with the methods that will be registered
		static luaL_Reg			windowsLibrary[];
		
		//! Lua functions
		static int	lua_getLocalizedString( lua_State* L );
		static int	lua_getCurrentFocus( lua_State* L );
		static int	lua_getSystemRegistry( lua_State* L );
		static int	lua_getUserRegistry( lua_State* L );
		static int	lua_getFont( lua_State* L );
		static int	lua_getWorkingDir( lua_State* L );
		static int	lua_getBinaryPath( lua_State* L );
		
		static int	lua_keyboardIsRegistered( lua_State* L );
		static int	lua_keyboardIsOpened( lua_State* L );
		static int	lua_keyboardOpen( lua_State* L );
		static int	lua_keyboardClose( lua_State* L );
		static int	lua_keyboardSetShift( lua_State* L );
		static int	lua_keyboardSetCaps( lua_State* L );
		static int	lua_keyboardIsShift( lua_State* L );
		static int	lua_keyboardIsCaps( lua_State* L );
		
		static int	lua_writeDebug( lua_State* L );
		static int	lua_pushEvent( lua_State* L );
		static int	lua_getMainFrame( lua_State* L );
		static int	lua_pause( lua_State* L );
		
		//! Print Function
		static void	lua_print( lua_State* L , _literal str , unsigned int length );
		
		//! Services
		static int	lua_startService( lua_State* L );
		static int	lua_closeService( lua_State* L );
		static int	lua_getServiceState( lua_State* L );
		static int	lua_getServiceTransferData( lua_State* L );
		static int	lua_setServiceHandler( lua_State* L );
		static int	lua_getServiceHandler( lua_State* L );
		
		//! system.exit() method
		static int	lua_exit( lua_State* L );
		
		//! Register DSWindows-Libs
		static int	lua_usingClass( lua_State* L );
		
		//! Pushes a table with all transfer data onto the given stack
		static void	pushTransferData( lua_State* L , _serviceTransfer transfer );
		
		//! registers system.* namespace
		void registerSystem();
		
		//! Parses the program header
		bool parseProgramHeader();
		
	public:
		
		//! Ctor
		_progLua( string prog );
		
		//! Main function that will be called at the start of execution (one shot)
		void main( _args args ) override ;
		
		//! Service functions
		void dispatch( _serviceId id , _serviceTransfer args ) override ;
		void process( _serviceId id , _serviceState& state ) override ;
		
		//! Returns the service data
		_serviceTransfer getServiceData( _serviceId id ) override ;
		
		//! Cleans up the Lua program and state
		void cleanUp() override ;
		
		//! VBL-function to collect garbage and reset lua_stack
		void frame( _int numRunningPrograms ) override ;
};

#endif