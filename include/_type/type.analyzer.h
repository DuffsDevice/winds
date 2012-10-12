#ifndef _WIN_T_ANALYZER_
#define _WIN_T_ANALYZER_

extern map<void*,_string> address2Name;
extern map<void*,_u32> address2Time;
extern map<void*,_u32> address2StartTime;

void startTimer( void* address );

void stopTimer( void* address );

void addMethod( void* address , _string name );

void printResults();

#endif