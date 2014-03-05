#ifndef _WIN_T_ANALYZER_
#define _WIN_T_ANALYZER_

#include "_type/type.h"

#ifdef DEBUG_PROFILING

extern "C" _u32 cpuGetTiming();

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
		_codeAnalyzer( _literal name ) :
			name( name )
			, startTime( cpuGetTiming() )
		{
			name2CallCount[name]++;
		}
		
		//! Destructor (submits execution times!)
		~_codeAnalyzer()
		{
			name2Time[this->name] += cpuGetTiming() - this->startTime;
		}
		
		static void printResults();
};

#else

class _codeAnalyzer{
	public:
		
		//! Dummy Ctor
		constexpr _codeAnalyzer( _literal ){}
		
		//! Dummy function
		static void printResults( _literal ){}
};

#endif

#endif