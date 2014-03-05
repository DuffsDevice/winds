#ifndef _WIN_T_PROGRAMARGS_
#define _WIN_T_PROGRAMARGS_

#include "_type/type.h"

class _programArgs : public _vector<string>
{
	public:
		
		//! Ctor
		_programArgs( _literal argumentString );
		_programArgs(){}
		
		//! Copy and Move Ctors
		_programArgs( _programArgs&& ) = default;
		_programArgs( const _programArgs& ) = default;
		
		//! Assignment operators
		_programArgs& operator=( _programArgs&& ) = default;
		_programArgs& operator=( const _programArgs& ) = default;
	
};

#endif