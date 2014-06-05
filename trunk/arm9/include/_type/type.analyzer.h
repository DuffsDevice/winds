#ifndef _WIN_T_ANALYZER_
#define _WIN_T_ANALYZER_

#include "_type/type.h"

#ifdef DEBUG_PROFILING

#include <memory>

class _codeAnalyzer
{
	private:
		
		// Pointer to the function
		_literal name;
		_tempTime startTime;
		
		static _map<_literal,_u32> name2Time;
		static _map<_literal,_u32> name2CallCount;
	
	public:
		
		//! Generic constructor!
		_codeAnalyzer( _literal name );
		
		//! Destructor (submits execution times!)
		~_codeAnalyzer();
		
		//! Print results of the speed analyzer
		static void printResults();
};

class _memoryAnalyzer
{
	private:
		
		_literal location;
		
	public:
		
		//! Table to track all allocations and deallocations
		static _map<void*,_literal>	ptr2location;
		
		//! Ctor
		constexpr _memoryAnalyzer( _literal location ) : location( location ) {}
		
		//! @see https://groups.google.com/forum/#!topic/comp.lang.c++.moderated/cIk4LjvBxIk
		template<typename T>
		T* operator <<( T* ptr ){
			ptr2location[ptr] = this->location;
			return ptr;
		}
		
		//! Print results of the memory leak detector
		static void printResults();
};

//! New overloads
void* operator new( size_t size );

//! Corresponding Delete overloads
void operator delete( void* ptr ) noexcept ;

//! New & Delete Macros that allow nice leak detections
#define S1(x) #x
#define S2(x) S1(x)
#define __LOCATION__ __FILE__ " : " S2(__LINE__)
#define new _memoryAnalyzer(__LOCATION__) << new

#else

//! Dummy Classes
class _codeAnalyzer{
	public:
		inline constexpr _codeAnalyzer( _literal ){}
		inline static void printResults( _literal ){}
};
class _memoryAnalyzer{
	public:
		inline static void printResults();
};

#endif // #ifdef DEBUG_PROFILING
#endif // Include once