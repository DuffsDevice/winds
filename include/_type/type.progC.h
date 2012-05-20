// Check if already included
#ifndef _WIN_T_CPROG_
#define _WIN_T_CPROG_

#include "_type/type.program.h"
using namespace std;

class _progC : public _program{
		
	private:
		
		void 		(_progC::*initFunc)( _cmdArgs& args );
		int 		(_progC::*mainFunc)( _cmdArgs& args );
		void 		(_progC::*destructFunc)();
		
	public:
		
		_progC( void (_progC::*init)(_cmdArgs&) , void (_progC::*destruct)() , int (_progC::*main)(_cmdArgs&) );
		
		~_progC();
		
		int main( _cmdArgs& args );
		
		void init( _cmdArgs& args );
		
};

#endif