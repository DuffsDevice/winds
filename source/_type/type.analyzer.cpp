#include "_type/type.h"

#include <nds/timers.h>
#include <stdio.h>
#include <nds/arm9/math.h>
#include <nds/arm9/console.h>

map<void*,_string> address2Name;
map<void*,_u32> address2Time;
map<void*,_u32> address2StartTime;

void startTimer( void* address )
{
	address2StartTime[address] = cpuGetTiming();
}

void stopTimer( void* address )
{
	address2Time[address] += cpuGetTiming() - address2StartTime[address];
}

void addMethod( void* address , _string name )
{
	address2Name[address] = name;
}

void printResults(){
	consoleClear();
	_u32 time = cpuGetTiming();
	for( pair<void*,_string> s : address2Name )
		printf("%s: %d = %d%%\n",s.second.c_str(),address2Time[s.first],div32( address2Time[s.first] , time/100 ) );
}