#ifndef _WIN_T_PROGRAM_HANDLE_
#define _WIN_T_PROGRAM_HANDLE_

#include "_type/type.h"
#include "_type/type.program.h"

class _programHandle
{
	private:
		
		mutable _program* ptr;
		
		// Checks if the pointer is still valid
		_program* validate(){
			if( !this->ptr )
				return nullptr;
			for( _programList::value_type& val : _program::getRunningPrograms() )
				if( val.first == this->ptr )
					return this->ptr;
			return this->ptr = nullptr;
		}
	
	public:
		
		//! Casts to _program*
		operator _program*(){
			return validate();
		}
		operator const _program*() const {
			return validate();
		}
		
		//! Check if the program to which the handle points still exists
		bool isValid(){
			return validate() != nullptr;
		}
		
		//! Dereferencing operator
		const _program& operator*(){ return *validate(); }
		
		//! Const Dereferencing operator
		const _program& operator*() const { return *validate(); }
		
		//! Dereferencing operator
		_program* operator->(){ return validate(); }
		
		//! Const Dereferencing operator
		const _program* operator->() const { return validate(); }

#endif