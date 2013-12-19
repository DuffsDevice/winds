#ifndef _WIN_T_SOUND_MSG_MIC_
#define _WIN_T_SOUND_MSG_MIC_

#include "_type/type.predefines.h"

struct _micRecordMsg{
	_s8*	buffer;
	_u32	bufferLength;
	_u16	freq;
	_u8		format;
};

struct _micBufferFullMsg{
	_s8*	buffer;
	_u32	length;
};

#endif