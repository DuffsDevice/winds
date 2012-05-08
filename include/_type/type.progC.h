// Check if already included
#ifndef _WIN_T_CPROG_
#define _WIN_T_CPROG_

#include "_type/type.program.h"
using namespace std;

class _progC : public _program{
		
	private:
		
		void 		(_progC::*initFunc)(_cmdArgs);
		int 		(_progC::*mainFunc)(_cmdArgs);
		
	public:
		
		_progC( void (_progC::*init)(_cmdArgs) , int (_progC::*main)(_cmdArgs) );
		
		int main( _cmdArgs args );
		
		void init( _cmdArgs args );
		
};

#endif