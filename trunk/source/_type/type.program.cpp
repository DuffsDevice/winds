#include "_type/type.program.h"
#include "_type/type.system.h"

void _program::init( _gadget* w , _cmdArgs& args  ){
	this->gadgetHost = w; this->init( args );
}

void _program::execute( _cmdArgs args ){
	_system_->executeProgram( this , args );
}

void _program::terminate(){
	_system_->terminateProgram( this ); // Kotzen
}