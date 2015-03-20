#ifndef _WIN_T_CPROG_
#define _WIN_T_CPROG_

#include <_type/type.program.h>

class _cProgram : public _program
{
	public:
		
		//! Ctor
		_cProgram() : _program( _programType::c ) { }
};

#endif