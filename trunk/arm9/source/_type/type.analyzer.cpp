#include "_type/type.h"

#ifdef DEBUG_PROFILING

#include <nds/timers.h>
#include <stdio.h>
#include <nds/arm9/math.h>
#include <nds/arm9/console.h>

_map<_literal,_u32>		_codeAnalyzer::name2Time;
_map<_literal,_u32>		_codeAnalyzer::name2CallCount;

void _codeAnalyzer::printResults(){
	consoleClear();
	//_u32 time = cpuGetTiming();
	for( _pair<_literal,_u32> s : name2Time )
	{
		printf("%-22s: %d\n %d: %d\n", s.first /* Name */ , s.second / name2CallCount[s.first] /* Timer per call */, name2CallCount[s.first] /* Number of calls */ , s.second );
		_system::submit();
		//printf("%s: %d = %d%%\n",s.second.c_str(),address2Time[s.first],div32( address2Time[s.first] , time/100 ) );
	}
}

#endif