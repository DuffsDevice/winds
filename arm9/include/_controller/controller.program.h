#ifndef _WIN_C_PROGRAM_
#define _WIN_C_PROGRAM_

#include "_type/type.h"
#include "_type/type.controller.h"
#include "_type/type.program.h"
#include "_type/type.program.executiondata.h"

// Type of a list of _programs and thier corresponding execution detail structs
typedef _list<_uniquePtr<_program>> _programList;

class _programController : public _controller
{
	private:
		
		//! Static List of running programs
		static _programList runningPrograms;
		static _programList programsToExecute;
		
		friend class _program;
		
		/**
		 * Executes a the supplied program
		 * @note The caller must not 'delete' the _program-instance supplied
		 */
		static bool executeProgram( _uniquePtr<_program> prog );
	
	public:
		
		//! Processes all programs
		static void		frame();
		
		//! Shuts down the program controller
		static void		end();
		
		//! Terminate them all
		static void		terminateAllPrograms();
		
		//! Get a list of all currently running programs
		static const _programList& getRunningPrograms(){
			return _programController::runningPrograms;
		}
		
		//! Check if a program still exists
		static bool isExistent( const _program* prog );
};

#endif