// Check if already included
#ifndef _WIN_T_CPROG_
#define _WIN_T_CPROG_

#include "_type/type.program.h"
using namespace std;

class _progC : public _program{
		
	private:
		
		void 		(_progC::*mainFunc)( _cmdArgs& args );
		void 		(_progC::*destructFunc)();
		
	public:
		
		_progC( void (_progC::*main)(_cmdArgs&) , void (_progC::*destruct)() );
		
		~_progC();
		
		void main( _cmdArgs& args );
};

#endif