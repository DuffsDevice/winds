#include "_type/type.program.h"
#include "_type/type.system.h"

void _program::main( _gadget* w , _cmdArgs& args  ){
	this->gadgetHost = w; this->main( args );
}

void _program::execute( _cmdArgs args ){
	_system::executeProgram( this , args );
}

void _program::terminate(){
	_system::terminateProgram( this ); // Kotzen
}