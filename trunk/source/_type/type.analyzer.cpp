#include "_type/type.h"
#include "_type/type.system.h"

#include <nds/timers.h>
#include <stdio.h>
#include <nds/arm9/math.h>
#include <nds/arm9/console.h>

_map<const _char*,_u32>		_codeAnalyzer::name2Time;
_map<const _char*,_u32>		_codeAnalyzer::name2CallCount;

void _codeAnalyzer::printResults(){
	consoleClear();
	//_u32 time = cpuGetTiming();
	for( pair<const _char*,_u32> s : name2Time )
	{
		printf("%-22s: %d\n %d: %d\n", s.first , s.second / name2CallCount[s.first] , name2CallCount[s.first] , s.second );
		_system::submit();
		//printf("%s: %d = %d%%\n",s.second.c_str(),address2Time[s.first],div32( address2Time[s.first] , time/100 ) );
	}
}