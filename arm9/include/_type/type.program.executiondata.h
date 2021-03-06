#ifndef _WIN_T_PROGRAM_EXECUTIONDATA_
#define _WIN_T_PROGRAM_EXECUTIONDATA_

#include <_type/type.h>
#include <_type/type.uniqueptr.h>
#include <_type/type.arguments.h>

// Struct that gives information about the state of a running program
struct _programExecutionData
{
	_tempTime			startTime;
	_args				argumentsForMain;
	struct{
		bool			markedForTermination : 1;
		bool			mainCalled : 1;
		mutable bool	isService : 1;
		mutable bool	usedByService : 1;
	}PACKED;
	
	//! Ctor
	_programExecutionData( _tempTime startTime , _args arguments ) :
		startTime( startTime )
		, argumentsForMain( move(arguments) )
		, markedForTermination( false )
		, mainCalled( false )
		, isService( false )
		, usedByService( false )
	{}
	
	//! Default Ctor
	_programExecutionData() :
		markedForTermination( false )
		, mainCalled( false )
		, isService( false )
		, usedByService( false )
	{}
};

#endif