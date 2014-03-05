#ifndef _WIN_T_PROGRAM_HANDLE_
#define _WIN_T_PROGRAM_HANDLE_

#include "_type/type.h"
#include "_type/type.program.h"

class _programHandle
{
	private:
		
		mutable const _program* ptr;
		
		//! Checks if the pointer is still valid
		const _program* validate() const {
			if( !this->ptr )
				return nullptr;
			for( const _programList::value_type& val : _program::getRunningPrograms() )
				if( (_program*)val.first == this->ptr )
					return this->ptr;
			return this->ptr = nullptr;
		}
	
	public:
		
		//! Casts to _program*
		operator const _program*() const {
			return validate();
		}
		
		//! Get pointer to program, if program still exists
		const _program* get() const {
			return validate();
		}
		
		//! Check if the program to which the handle points still exists
		bool isValid() const {
			return validate() != nullptr;
		}
		
		//! Makes the handle point to the first program in the list of running programs
		const _program* front(){
			if( !_program::getRunningPrograms().empty() )
				return this->ptr = _program::getRunningPrograms().front().first;
			return this->ptr = nullptr;
		}
		
		//! Makes the handle point to the first program in the list of running programs
		const _program* back(){
			if( !_program::getRunningPrograms().empty() )
				return this->ptr = _program::getRunningPrograms().back().first;
			return this->ptr = nullptr;
		}
		
		//! Modifies the handle to point to the next program in the list
		const _program* next(){
			if( !this->ptr )
				return this->front();
			_programList::const_iterator it = find_if(
				_program::getRunningPrograms().cbegin()
				, _program::getRunningPrograms().cend()
				, [this]( const _programList::value_type& prog )->bool{ return prog.first == this->ptr; }
			);
			_programList::const_iterator end = _program::getRunningPrograms().cend();
			if( it != end && ++it != end )
				return this->ptr = it->first;
			return this->front();
		}
		
		//! Modifies the handle to point to the next program in the list
		const _program* prev(){
			if( !this->ptr )
				return this->back();
			_programList::const_reverse_iterator it = find_if(
				_program::getRunningPrograms().crbegin()
				, _program::getRunningPrograms().crend()
				, [this]( const _programList::value_type& prog )->bool{ return prog.first == this->ptr; }
			);
			_programList::const_reverse_iterator end = _program::getRunningPrograms().crend();
			if( it != end && ++it != end )
				return this->ptr = it->first;
			return this->front();
		}
		
		//! Dereferencing operator
		const _program& operator*() const { return *validate(); }
		
		//! Dereferencing operator
		const _program* operator->() const { return validate(); }
};

#endif