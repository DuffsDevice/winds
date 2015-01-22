#include <_controller/controller.execution.h>
#include <_controller/controller.gui.h>
#include <_controller/controller.registry.h>
#include <_type/type.program.h>

void _executionController::removeProgramsOfGadgetHost( _gadgetScreen* host )
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

_serviceState _executionController::closeService( _serviceId id ){
	for( _pair<_serviceId,_serviceData>& s : openServices ){
		if( s.first == id ){
			_serviceState temp = s.second.state;
			s.second.state = _serviceState::terminate;
			return temp; // Return the state of the service before it was destroyed
		}
	}
	return _serviceState::undefined;
}

bool _executionController::isExistent( const _program* prog )
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

_program* _executionController::isExistent( const string& path , bool serviceProgramOnly )
{
	// Iterate over currently running programs
	for( const _programList::value_type& val : programsToExecute )
		if( val->getPath() == path && ( !serviceProgramOnly || val->getExecutionData().isService ) )
			return val;
	// .. and over programs to be executed
	for( const _programList::value_type& val : runningPrograms )
		if( val->getPath() == path && ( !serviceProgramOnly || val->getExecutionData().isService ) )
			return val;
	return nullptr;
}

_program* _executionController::execute( _uniquePtr<_program> prog , _args args )
{
	if( !prog || isExistent( prog ) )
		return nullptr;
	
	prog->prepareExecution( move(args) );
	
	// Push it in the list of running programs
	programsToExecute.emplace_back(	move(prog) );
	
	return prog;
}

_program* _executionController::executeAsService( _uniquePtr<_program> prog )
{
	if( !prog || isExistent( prog->getPath() , true ) )
		return nullptr;
	
	// Set program to act like a service
	prog->getExecutionData().isService = true;
	
	// Push it in the list of running programs
	programsToExecute.emplace_back(	move(prog) );
	
	return prog;
}

void _executionController::terminateAllPrograms(){
	std::for_each(
		runningPrograms.begin()
		, runningPrograms.end()
		, []( _programList::value_type& p ){ p->terminate(); }
	);
	frame();
	runningPrograms.clear();
	programsToExecute.clear();
	openServices.clear();
}

void _executionController::end(){
	terminateAllPrograms();
}

_serviceState _executionController::getServiceState( _serviceId id )
{
	// Iterate over open services
	auto iter = find_if(
		openServices.begin()
		, openServices.end()
		, [id]( const _pair<_serviceId,_serviceData>& s )->bool{ return s.first == id; }
	);
	
	if( iter != openServices.end() )
		return iter->second.state;
	
	return _serviceState::undefined;
}

_serviceTransfer _executionController::getServiceTransferData( _serviceId id )
{
	// Iterate over open services
	auto iter = find_if(
		openServices.begin()
		, openServices.end()
		, [id]( const _pair<_serviceId,_serviceData>& s )->bool{ return s.first == id; }
	);
	
	if( iter != openServices.end() )
		return move(iter->second.correspProg->getServiceData( id ));
	
	return _serviceTransfer();
}

void _executionController::frame()
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
					if( data.markedForTermination && !data.usedByService ){
						if( data.mainCalled )
							p->cleanUp();
						return true;
					}
					
					data.usedByService = false;
					
					// Call main()-Method if necessary
					if( !data.mainCalled && !data.isService )
						p->callMain();
					
					// Call the Vertical Blank Handler
					p->frame( numRunningPrograms ); // Run it
					
					return false;
				}
		)
		, runningPrograms.end()
	);
	
	// Iterate over open services
	openServices.erase(
		remove_if(
			openServices.begin()
			, openServices.end()
			, []( _pair<_serviceId,_serviceData>& s )
				->bool
				{
					_program* prog = s.second.correspProg;
					if( !prog ) // Make sure we don't have a system error
						return true;
					
					if( s.second.state == _serviceState::init ){
						prog->callDispatch( s.first /* Service Id */ , move(s.second.arguments) );
						s.second.state = _serviceState::main;
					}
					else if( s.second.state == _serviceState::main )
						prog->callProcess( s.first /* Service Id */ , s.second.state );
					
					// Delete service if its terminated
					if( s.second.state == _serviceState::terminate )
						return true;
					
					// Make sure nobody kills our program, which is still used by a service!
					prog->getExecutionData().usedByService = true;
					
					return false;
				}
		)
		, openServices.end()
	);
}

_serviceId _executionController::startService( const string& identifier , _serviceTransfer&& arguments )
{
	string path		= _registryController::getPackagePath( identifier );
	_program* prog	= _executionController::executeAsService( _program::fromFile( path ) );
	
	// Check if we have an executeable that acts as the service handler
	if( !prog )
		return false;
	
	// New service Id
	_serviceId newId = 1;
	
	for( _pair<_serviceId,_serviceData>& value : openServices )
		if( value.first == newId )
			newId++;
	
	openServices[newId] = _serviceData( prog , move(arguments) );
	
	return newId;
}

_programList		_executionController::runningPrograms;
_programList		_executionController::programsToExecute;
_openServiceList	_executionController::openServices;