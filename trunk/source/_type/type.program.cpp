#include "_type/type.program.h"
#include "_type/type.progLua.h"
#include "_type/type.progC.h"
#include "_type/type.system.h"

#include "_resource/PROG_Explorer.h"
#include "PROG_exampleprogram_bin.h"
#include "PROG_pong_bin.h"
#include "PROG_paint_bin.h"

void _program::main( _gadget* w , _cmdArgs&& args  ){
	this->gadgetHost = w;
	this->internalMain( move( args ) );
}

void _program::execute( _cmdArgs&& args ){
	_system::executeProgram( this , move( args ) );
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
	{
		string str = (const _char*)PROG_exampleprogram_bin;
		str.resize( PROG_exampleprogram_bin_size );
		return new _progLua( move( str ) );
	}
	
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/paint.exe" ) )
	{
		string str = (const _char*)PROG_paint_bin;
		str.resize( PROG_paint_bin_size );
		return new _progLua( move( str ) );
	}
	
	if( fn == _direntry::replaceASSOCS( "%SYSTEM%/pong.exe" ) )
	{
		string str = (const _char*)PROG_pong_bin;
		str.resize( PROG_pong_bin_size );
		return new _progLua( move( str ) );
	}
	
	return nullptr;
}