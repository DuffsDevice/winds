// Check if already included
#ifndef _WIN_T_CPROG_
#define _WIN_T_CPROG_

#include "_type/type.program.h"
#include "_type/type.system.h"

class _progC : public _program
{
	private:
		
		void internalMain( _cmdArgs args ){
			this->main( move(args) );
		}
		
	protected:
		
		virtual void main( _cmdArgs args ) = 0;
		virtual void destruct(){};
		
	public:
		
		//! Ctor
		_progC() : _program( _programType::progC ) { }
		
		//! Dtor
		~_progC(){
			this->destruct();
		}
};

#endif