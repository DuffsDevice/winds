#ifndef _WIN_T_SOUND_MSG_
#define _WIN_T_SOUND_MSG_

#include "_type/type.predefines.h"
#include "_type/type.sound.msg.channel.h"
#include "_type/type.sound.msg.mic.h"

enum class _soundDataMsgType : _u8
{
	channelSet,			// messages to arm7
	micRecord,
	micBufferFull,
	
	channelStopped,		// messages to arm9
	bufferFill,
	integer
};

struct _soundDataMsg
{
	_soundDataMsgType type;
	
	union
	{
		_channelSetMsg		channelSet;
		_channelStoppedMsg	channelStopped;
		_fillingParams		bufferFill;
		_micBufferFullMsg	micBufferFull;
		_micRecordMsg		micRecord;
		int					integer;
	};
	
	//! We need a ctor here because _channelSetMsg has a non-trivial constructor
	_soundDataMsg(){}
	
	//! Named constructors for different scenarios
	static _soundDataMsg channelSetMsg( _channelSetMsg param ){
		_soundDataMsg msg;
		msg.type = _soundDataMsgType::channelSet;
		msg.channelSet = param;
		return msg;
	}
	
	static _soundDataMsg micBufferFullMsg( _micBufferFullMsg param ){
		_soundDataMsg msg;
		msg.type = _soundDataMsgType::micBufferFull;
		msg.micBufferFull = param;
		return msg;
	}
	
	static _soundDataMsg micRecordMsg( _micRecordMsg param ){
		_soundDataMsg msg;
		msg.type = _soundDataMsgType::micRecord;
		msg.micRecord = param;
		return msg;
	}
	
	static _soundDataMsg channelStoppedMsg( _channelStoppedMsg param ){
		_soundDataMsg msg;
		msg.type = _soundDataMsgType::channelStopped;
		msg.channelStopped = param;
		return msg;
	}
	
	static _soundDataMsg bufferFillMsg( _fillingParams param ){
		_soundDataMsg msg;
		msg.type = _soundDataMsgType::bufferFill;
		msg.bufferFill = param;
		return msg;
	}
	
	static _soundDataMsg integerMsg( int param ){
		_soundDataMsg msg;
		msg.type = _soundDataMsgType::integer;
		msg.integer = param;
		return msg;
	}
};

enum class _soundCommand : _u8
{
	// Enable - Disable
	soundEnable,
	soundDisable,
	
	channelKill,
	channelStop,
	channelPlay,
	
	micStop,
	soundMute,
	soundUnMute,
	
	// Setters
	setVolume,
	setPanning,
	setFrequency,
	setDutyCycle,
	
	// Getters
	getVolume,
	getPanning,
	getFrequency,
	getDutyCycle,
	
	// Check if enabled
	isSoundEnabled,
	isSoundMuted,
	isChannelEnabled,
	isChannelPlaying,
};

struct _soundCommandMsg
{
	union{
		struct{
			_soundCommand	cmd;
			_s8				channel;
			_s16			value;
		} PACKED ;
		_u32				sum;
	};
	
	_soundCommandMsg( _s32 sum ) :
		sum( sum )
	{}
	
	_soundCommandMsg( _soundCommand cmd , _u8 channel , _s16 value = 0 ) :
		cmd( cmd )
		, channel( channel )
		, value( value )
	{}
};

union _soundCommandResult
{
	_u32	sum;
	_s32	value;
	
	//! Ctors
	_soundCommandResult( _s32 value ) : 
		value( value )
	{}
	_soundCommandResult(){}
	
	//! Casts
	operator _s32(){ return this->value; }
	operator _u32(){ return this->sum; }
};

#endif