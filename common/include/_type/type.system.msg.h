#ifndef _WIN_T_SYSTEM_MSG_
#define _WIN_T_SYSTEM_MSG_

#include <_type/type.predefines.h>
#include <string.h>

enum class _systemDataMsgType : _u8
{
	cpuUsage,
	debug
};

struct _systemDataMsg
{
	_systemDataMsgType type;
	
	union
	{
		char		debugMessage[64];
		int			cpuUsage;
	};
	
	//! We need a ctor here because _channelSetMsg has a non-trivial constructor
	_systemDataMsg(){}
	
	//! Named constructors for different scenarios
	static _systemDataMsg debugMsg( char param[] ){
		_systemDataMsg msg;
		msg.type = _systemDataMsgType::debug;
		strcpy( msg.debugMessage , param );
		return msg;
	}
	
	static _systemDataMsg cpuUsageMsg( int param ){
		_systemDataMsg msg;
		msg.type = _systemDataMsgType::cpuUsage;
		msg.cpuUsage = param;
		return msg;
	}
};

#endif