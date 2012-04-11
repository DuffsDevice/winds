// Check if already included
#ifndef _WIN_T_CPROG_
#define _WIN_T_CPROG_

#include "_type/type.program.h"
using namespace std;

class _cmdArgs{};

class _progC : public _program{
	
	public:
		
		typedef int	(_progC::*_progfunc)( _cmdArgs args );
		
	private:
		
		_progfunc	initFunc;
		_progfunc	mainFunc;
		
	public:
		
		_progC( _progfunc init , _progfunc main );
		
		void run();
		
};

#endif