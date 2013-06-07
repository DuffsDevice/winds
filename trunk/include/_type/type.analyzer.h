#ifndef _WIN_T_ANALYZER_
#define _WIN_T_ANALYZER_

extern "C" _u32 cpuGetTiming();

class _codeAnalyzer
{
	private:
		
		// Pointer to the function
		const _char* name;
		_tempTime startTime;
		
		static _map<const _char*,_u32> name2Time;
		static _map<const _char*,_u32> name2CallCount;
	
	public:
		
		//! Generic constructor!
		noinline _codeAnalyzer( const _char* name ) :
			name( name )
			, startTime( cpuGetTiming() )
		{
			name2CallCount[name]++;
		}
		
		//! Destructor (submits execution times!)
		noinline ~_codeAnalyzer()
		{
			name2Time[this->name] += cpuGetTiming() - this->startTime;
		}
		
		static void printResults();
};

#endif