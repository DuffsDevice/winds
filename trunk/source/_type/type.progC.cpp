#include "_type/type.progC.h"
#include "_type/type.system.h"

_progC::_progC( void (_progC::*init)(_cmdArgs) , int (_progC::*main)(_cmdArgs) ) :
	_program( progC )
	, initFunc( init )
	, mainFunc( main )
{ }

int _progC::main( _cmdArgs args ){
	if( this->mainFunc )
		return (this->*mainFunc)( args );
	return 0;
}

void _progC::init( _cmdArgs args ){
	if( this->initFunc )
		(this->*initFunc)( args );
}