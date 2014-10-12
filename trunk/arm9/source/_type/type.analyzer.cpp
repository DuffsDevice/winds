#include <_type/type.h>
#undef new

#ifdef _WIN_CONFIG_BUILD_DEBUG_PROFILING_

#include <nds/timers.h>
#include <nds/arm9/math.h>
#include <nds/arm9/console.h>
#include <malloc.h>
#include <_controller/controller.debug.h>
#include <_controller/controller.timer.h>

_map<_literal,_u32>		_codeAnalyzer::name2Time;
_map<_literal,_u32>		_codeAnalyzer::name2CallCount;

_codeAnalyzer::~_codeAnalyzer(){
	name2Time[this->name] += _timerController::getMicroTime() - this->startTime;
}

_codeAnalyzer::_codeAnalyzer( _literal name ) :
	name( name )
	, startTime( _timerController::getMicroTime() )
{
	name2CallCount[name]++;
}

void _codeAnalyzer::printResults(){
	printf("Speed Results:\n");
	//consoleClear();
	for( _pair<_literal,_u32> s : name2Time )
	{
		printf("%-22s: %d\n %d: %d\n", s.first /* Name */ , s.second / name2CallCount[s.first] /* Timer per call */, name2CallCount[s.first] /* Number of calls */ , s.second );
		//_debugController::submit();
	}
}

_map<void*,_literal> _memoryAnalyzer::ptr2location;

void _memoryAnalyzer::printResults(){
	printf("Memory Results:\n");
	//consoleClear();
	for( _pair<void*,_literal> s : ptr2location )
	{
		printf("Leak at %s\n", s.second );
		_debugController::submit();
	}
}

void* operator new(size_t size){
	return malloc( size );
}

void operator delete(void* ptr) noexcept {
	auto iter = _memoryAnalyzer::ptr2location.find( ptr );
	if( iter != _memoryAnalyzer::ptr2location.end() ){
		_memoryAnalyzer::ptr2location.erase( iter );
		free( ptr );
	}
	else{
		//printf("Deleting undefined\n");
		//_debugController::debug( "Deleting undefined reference!" );
	}
}

#endif