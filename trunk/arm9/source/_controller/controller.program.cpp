#include <_controller/controller.program.h>
#include <_controller/controller.gui.h>
#include <_type/type.program.h>

void _programController::removeProgramsOfGadgetHost( _gadgetScreen* host )
{
	std::for_each(
		runningPrograms.begin()
		, runningPrograms.end()
		, [host]( _programList::value_type& p )
			{
				// Delete the program if it is marked for termination
				if( p->getMainFrame() && p->getMainFrame()->getParent() == host )
					p->terminate();
			}
	);
}

bool _programController::isExistent( const _program* prog )
{
	// Iterate over currently running programs
	for( const _programList::value_type& val : programsToExecute )
		if( (_program*)val == prog )
			return true;
	// .. and over programs to be executed
	for( const _programList::value_type& val : runningPrograms )
		if( (_program*)val == prog )
			return true;
	return false;
}

bool _programController::executeProgram( _uniquePtr<_program> prog )
{
	if( isExistent( prog ) )
		return false;
	
	// Push it in the list of running programs
	programsToExecute.emplace_back(	move(prog) );
	return true;
}

void _programController::terminateAllPrograms(){
	std::for_each(
		runningPrograms.begin()
		, runningPrograms.end()
		, []( _programList::value_type& p ){ p->terminate(); }
	);
	frame();
	runningPrograms.clear();
	programsToExecute.clear();
}

void _programController::end(){
	terminateAllPrograms();
}

void _programController::frame()
{
	// Move programs to execute into the list of actually running programs
	for( _uniquePtr<_program>& prog : programsToExecute )
		runningPrograms.emplace_back( move(prog) );
	
	// Clear buffer
	programsToExecute.clear();
	
	// Cache the number of running programs
	_u32	numRunningPrograms = runningPrograms.size();
	
	// Iterate over programs (run their vbl-Methods etc...)
	runningPrograms.erase(
		remove_if(
			runningPrograms.begin()
			, runningPrograms.end()
			, [numRunningPrograms]( _programList::value_type& p )
				->bool
				{
					const _programExecutionData& data = p->getExecutionData();
					
					// Delete the program if it is marked for termination
					if( data.markedForTermination ){
						if( data.mainCalled )
							p->cleanUp();
						return true;
					}
					
					// Call main()-Method if necessary
					if( !data.mainCalled )
						p->callMain();
					
					// Call the Vertical Blank Handler
					p->frame( numRunningPrograms ); // Run it
					
					return false;
				}
		)
		, runningPrograms.end()
	);
}

_programList _programController::runningPrograms;
_programList _programController::programsToExecute;