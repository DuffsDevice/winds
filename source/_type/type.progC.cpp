#include "_type/type.progC.h"
#include "_type/type.system.h"

_progC::_progC( _progfunc init , _progfunc main ) :
	_program( progC )
	, initFunc( init )
	, mainFunc( main )
{ }

void _progC::run(){
	if( this->initFunc )
		(this->*initFunc)( _cmdArgs() );
	if( this->mainFunc )
		(this->*mainFunc)( _cmdArgs() );
}