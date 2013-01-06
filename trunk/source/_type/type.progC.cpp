#include "_type/type.progC.h"
#include "_type/type.system.h"

_progC::_progC( void (_progC::*main)(_cmdArgs&) , void (_progC::*destruct)() ) :
	_program( _programType::progC )
	, mainFunc( main )
	, destructFunc( destruct )
{ }

void _progC::main( _cmdArgs& args ){
	if( this->mainFunc )
		(this->*mainFunc)( args );
}

_progC::~_progC(){
	if( this->destructFunc )
		(this->*destructFunc)();
}