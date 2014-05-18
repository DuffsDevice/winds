#ifndef _WIN_T_PROGRAM_HANDLE_
#define _WIN_T_PROGRAM_HANDLE_

#include "_type/type.h"

class _program;

class _programHandle
{
	private:
		
		mutable _program* ptr;
		
		//! Checks if the pointer is still valid
		_program* validate() const ;
	
	public:
		
		//! Ctor
		_programHandle( _program* program ) :
			ptr( program )
		{}
		
		//! Casts to _program*
		operator _program*() const {
			return validate();
		}
		
		//! Get pointer to program, if program still exists
		_program* get() const {
			return validate();
		}
		
		//! Check if the program to which the handle points still exists
		bool isValid() const {
			return validate() != nullptr;
		}
		
		//! Makes the handle point to the first program in the list of running programs
		_program* front();
		
		//! Makes the handle point to the last program in the list of running programs
		_program* back();
		
		//! Modifies the handle to point to the next program in the list
		_program* next();
		
		//! Modifies the handle to point to the previous program in the list
		_program* prev();
		
		//! Dereferencing operator
		_program& operator*() const { return *validate(); }
		
		//! Dereferencing operator
		_program* operator->() const { return validate(); }
};

#endif