#ifndef _WIN_T_SERVICE_
#define _WIN_T_SERVICE_

#include <_type/type.h>
#include <_type/type.assocvector.h>
#include <_type/type.arguments.h>
#include <_type/type.uniqueptr.h>
#include <_type/type.bitmap.h>

class _program;

typedef _s32 _serviceId;

//! Struct that allows exchange of various data types
struct _serviceTransfer
{
	_uniquePtr<_args>	arguments;
	_uniquePtr<string>	strData;
	_uniquePtr<_bitmap>	bitmapData;
	_int				returnData;
	_int				int1Data;
	_int				int2Data;
	float				float1Data;
	float				float2Data;
	
	//! Ctor
	_serviceTransfer() :
		returnData( 0 )
		, int1Data( 0 )
		, int2Data( 0 )
		, float1Data( 0 )
		, float2Data( 0 )
	{}
};

//! Enumerates all possible states of a service
enum class _serviceState{
	undefined,
	init,
	main,
	success,
	error,
	terminate
};

//! Data that has to be saved for every instance of a service
struct _serviceData
{
	_program*			correspProg;
	_serviceTransfer	arguments;
	_serviceState		state;
	_serviceTransfer	returnData;
	
	//! Ctor
	_serviceData( _program* prog = nullptr , _serviceTransfer arguments = _serviceTransfer() ) :
		correspProg( prog )
		, arguments( move(arguments) )
		, state( _serviceState::init )
		, returnData( _serviceTransfer() )
	{}
};

// Type of a list of open services, each having a service identifier and the programm it corresponds to
typedef _assocVector<_serviceId,_serviceData>	_openServiceList;

extern _toStr<_serviceState> 	serviceState2string;
extern _fromStr<_serviceState>	string2serviceState;

#endif