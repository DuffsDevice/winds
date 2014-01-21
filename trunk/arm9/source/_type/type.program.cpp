#include "_type/type.program.h"
#include "_type/type.program.lua.h"
#include "_type/type.program.c.h"
#include "_type/type.system.h"

#include "_resource/resource.program.explorer.h"
#include "program_example_bin.h"
#include "program_pong_bin.h"
#include "program_paint_bin.h"

_programList	_program::globalPrograms;
_programList	_program::globalProgramsToExecute;

void _program::main( _gadget* w , _cmdArgs args  ){
	this->gadgetHost = w;
	this->internalMain( move( args ) );
}

int cnt = 0;

void _program::execute( _cmdArgs args )
{
	// Execute main
	this->main( _system::_gadgetHost_ , move(args) );
	
	// Push it in the list of running programs
	_program::globalProgramsToExecute.push_back(
		make_pair(
			this
			, _programData(
				{ false , _system::getHighResTime() }
			)
		)
	);
}

void _program::terminate()
{
	for( auto& p : _program::globalPrograms )
		if( (_program*)p.first == this )
		{
			p.second.autoDelete = true;
			break;
		}
}

void _program::terminateAllPrograms()
{
	for( auto& val : _program::globalPrograms )
		delete val.first;
	for( auto& val : _program::globalProgramsToExecute )
		delete val.first;
	_program::globalPrograms.clear();
	_program::globalProgramsToExecute.clear();
}

void _program::runPrograms()
{
	// Move timers to execute
	move( _program::globalProgramsToExecute.begin() , _program::globalProgramsToExecute.end() , std::back_inserter( _program::globalPrograms ) );
	
	// Clear buffer
	_program::globalProgramsToExecute.clear();
	
	printf("Programs: %d\n",_program::globalPrograms.size());
	
	_program::globalPrograms.erase(
		remove_if( _program::globalPrograms.begin() , _program::globalPrograms.end() , 
			[]( _programList::value_type& p )->bool
			{
				if( p.first && !p.second.autoDelete ){
					p.first->internalVbl(); // Run it
					return false;
				}
				delete p.first;
				return true;
			}
		)
		, _program::globalPrograms.end()
	);
}

_program* _program::fromFile( string filename )
{
	_direntry d = _direntry( filename );
	
	string fn = d.getFileName();
	
	// Convert to lowerspace
	transform( fn.begin() , fn.end() , fn.begin() , ::tolower );
	
	_program* result = nullptr;
	
	// For the case that the program exists
	if( d.isExisting() )
		result = new _progLua( d.readString() );
	else if( fn == _direntry::replaceASSOCS( "%SYSTEM%/explorer.exe" ) )
		result = new PROG_Explorer();
	else if( fn == _direntry::replaceASSOCS( "%SYSTEM%/exampleprogram.exe" ) ){
		string str = (const _char*)program_example_bin;
		str.resize( program_example_bin_size );
		result = new _progLua( move(str) );
	}
	else if( fn == _direntry::replaceASSOCS( "%SYSTEM%/paint.exe" ) ){
		string str = (const _char*)program_paint_bin;
		str.resize( program_paint_bin_size );
		result = new _progLua( move(str) );
	}
	else if( fn == _direntry::replaceASSOCS( "%SYSTEM%/pong.exe" ) ){
		string str = (const _char*)program_pong_bin;
		str.resize( program_pong_bin_size );
		result = new _progLua( move(str) );
	}
	
	result->setPath(fn);
	
	return result;
}