#include "_type/type.program.h"
#include "_type/type.progLua.h"
#include "_type/type.progC.h"
#include "_type/type.system.h"

#include "_resource/PROG_Explorer.h"
#include "program_bin.h"

void _program::main( _gadget* w , _cmdArgs& args  ){
	this->gadgetHost = w; this->main( args );
}

void _program::execute( _cmdArgs args ){
	_system::executeProgram( this , args );
}

void _program::terminate(){
	_system::terminateProgram( this ); // Kotzen
}


_program* _program::fromFile( string filename )
{
	_direntry d = _direntry( filename );
	
	string fn = d.getFileName();
	
	// Convert to lowerspace
	transform( fn.begin() , fn.end() , fn.begin() , ::tolower );
	
	// For the case that the program exists
	if( d.isExisting() )
		return new _progLua( d.readString() );
	
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/explorer.exe" ) )
		return new PROG_Explorer();
	
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/exampleprogram.exe" ) )
		return new _progLua( (const char*)program_bin );
	
	return nullptr;
}