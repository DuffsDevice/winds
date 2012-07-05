#include "_type/type.program.h"
#include "_type/type.system.h"

void _program::init( _windows* w , _cmdArgs& args  ){
	this->windowsInstance = w; this->init( args );
}

void _program::execute( _cmdArgs args ){
	_system_->executeProgram( this , args );
}