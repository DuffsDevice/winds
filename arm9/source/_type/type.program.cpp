#include "_type/type.program.h"
#include "_type/type.program.lua.h"
#include "_type/type.program.c.h"
#include "_type/type.system.h"

#include "_resource/PROG_Explorer.h"
#include "PROG_exampleprogram_bin.h"
#include "PROG_pong_bin.h"
#include "PROG_paint_bin.h"

_programList	_program::globalPrograms;
_programList	_program::globalProgramsToExecute;

void _program::main( _gadget* w , _cmdArgs args  ){
	this->gadgetHost = w;
	this->internalMain( move( args ) );
}

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

void _program::runPrograms()
{
	// Move timers to execute
	move( _program::globalProgramsToExecute.begin() , _program::globalProgramsToExecute.end() , std::back_inserter( _program::globalPrograms ) );
	
	// Clear buffer
	_program::globalProgramsToExecute.clear();
	
	_program::globalPrograms.erase(
		remove_if( _program::globalPrograms.begin() , _program::globalPrograms.end() , 
			[]( _programList::value_type& p )->bool
			{
				if( !p.first.data() )
					return true;
				else if( p.second.autoDelete )
					return true;
				// Run it
				p.first->internalVbl();
				
				return false;
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
		string str = (const _char*)PROG_exampleprogram_bin;
		str.resize( PROG_exampleprogram_bin_size );
		result = new _progLua( move(str) );
	}
	else if( fn == _direntry::replaceASSOCS( "%SYSTEM%/paint.exe" ) ){
		string str = (const _char*)PROG_paint_bin;
		str.resize( PROG_paint_bin_size );
		result = new _progLua( move(str) );
	}
	else if( fn == _direntry::replaceASSOCS( "%SYSTEM%/pong.exe" ) ){
		string str = (const _char*)PROG_pong_bin;
		str.resize( PROG_pong_bin_size );
		result = new _progLua( move(str) );
	}
	
	result->setPath(fn);
	
	return result;
}