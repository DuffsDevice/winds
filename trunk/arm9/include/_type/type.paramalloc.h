#ifndef _WIN_T_PARAMALLOC_
#define _WIN_T_PARAMALLOC_

#include <_type/type.h>

/**
 * Class that will used as parameter type quickly allocate
 * a copy of the passed object or moves it into a new allocation
 * 
 * @note This is especially useful with subclasses that have to get stored as a pointer
 */
template<class T>
class _paramAlloc
{
	private:
		
		T*	ptr;
		_u8	passed;
	
	public:
		
		//! Ctor that will allocate the object
		template<typename T2>
		_paramAlloc( T2&& obj ) :
			passed( false )
		{
			typedef typename std::remove_reference<T2>::type T3;
			unused typedef decltype( std::is_convertible<T3,T>::value ) check; // Check if T2 can be casted to T
			ptr = (T*) new T3( (T2&&)obj );
		}
		
		template<typename T2>
		_paramAlloc( T2* obj ) = delete;
		
		//! Move and copy ctors
		_paramAlloc( const _paramAlloc& ) = delete;
		_paramAlloc( _paramAlloc&& from ) :
			ptr( from.ptr ) , passed( from.passed )
		{
			from.passed = true;
			from.ptr = nullptr;
		}
		
		//! Get the now allocated pointer
		operator T*(){ this->passed = true; return this->ptr; }
		operator bool(){ return this->ptr != nullptr; }
		
		//! Named accessors
		bool isValid(){ return this->ptr != nullptr; }
		T* get(){ this->passed = true; return this->ptr; }
		
		//! Dtor
		~_paramAlloc(){
			if( !this->passed && this->ptr )
				delete this->ptr;
		}
		
		//! There is no assign!
		_paramAlloc& operator=( const _paramAlloc& ) = delete;
		_paramAlloc& operator=( _paramAlloc&& ) = delete;
};

#endif