#ifndef _WIN_T_PROGRAMARGS_
#define _WIN_T_PROGRAMARGS_

#include <_type/type.h>

class _args : public _vector<string>
{
	public:
		
		//! Ctor
		_args( _literal argumentString );
		_args(){}
		
		//! Copy and Move Ctors
		_args( _args&& ) = default;
		_args( const _args& ) = default;
		
		//! Assignment operators
		_args& operator=( _args&& ) = default;
		_args& operator=( const _args& ) = default;
		
		//! Splits a command in (executeable path;arguments)
		static _pair<string,string> splitCommand( const string& cmd );
};

#endif