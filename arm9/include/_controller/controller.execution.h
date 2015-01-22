#ifndef _WIN_C_EXECUTION_
#define _WIN_C_EXECUTION_

#include <_type/type.h>
#include <_type/type.controller.h>
#include <_type/type.screen.gadget.h>
#include <_type/type.service.h>
#include <_type/type.program.h>
#include <_type/type.program.executiondata.h>

// Type of a list of _programs and their corresponding execution detail structs
typedef _list<_uniquePtr<_program>>			_programList;

class _executionController : public _controller
{
	private:
		
		//! Static List of running programs
		static _programList 	runningPrograms;
		static _programList		programsToExecute;
		static _openServiceList	openServices;
		
	public:
		
		/**
		 * Executes the supplied program
		 * @return a pointer to the executed program, or nullptr if the program could not be executed
		 */
		static _program*		execute( _uniquePtr<_program> prog , _args args = _args() );
		/**
		 * Executes the supplied program as service, that is without
		 * calling main() and therefore without parameters
		 * @return a pointer to the executed program, or nullptr if the program could not be executed
		 */
		static _program*		executeAsService( _uniquePtr<_program> prog );
		
		//! Processes all programs
		static void				frame();
		
		//! Shuts down the program controller
		static void				end();
		
		//! Terminate all running programms (and services running on them)
		static void				terminateAllPrograms();
		
		//! Get a list of all currently running programs
		static const _programList& getRunningPrograms(){
			return _executionController::runningPrograms;
		}
		
		//! Check if a program still exists
		static bool				isExistent( const _program* prog );
		static _program*		isExistent( const string& path , bool serviceProgramOnly = false ); // returns the program with the specified path
		
		//! Removes all programs that are dependent of the supplied gadget host
		static void				removeProgramsOfGadgetHost( _gadgetScreen* host );
		
		//! Executes a service (and returns the id of the open service, (-1) if it could not be started)
		static _serviceId		startService( const string& identifier , _serviceTransfer&& data );
		
		//! Returns the state of a given service
		static _serviceState	getServiceState( _serviceId id );
		
		//! Closes a service and cleares all its used data
		//! Returns the state the service was in before it terminated
		static _serviceState	closeService( _serviceId id );
		
		
		//! Methods to receive data from a given service
		static _serviceTransfer	getServiceTransferData( _serviceId id );
};

#endif