// Check if already included
#ifndef _WIN_T_CPROG_
#define _WIN_T_CPROG_

#include "_type/type.program.h"

class _progC : public _program{
		
	private:
		
		void 		(_progC::*mainFunc)( _cmdArgs&& args );
		void 		(_progC::*destructFunc)();
		
	public:
		
		_progC( void (_progC::*main)(_cmdArgs&&) , void (_progC::*destruct)() ) :
			_program( _programType::progC )
			, mainFunc( main )
			, destructFunc( destruct )
		{ }

		void main( _cmdArgs&& args ){
			if( this->mainFunc )
				(this->*mainFunc)( move( args ) );
		}

		~_progC(){
			if( this->destructFunc )
				(this->*destructFunc)();
		}
};

#endif