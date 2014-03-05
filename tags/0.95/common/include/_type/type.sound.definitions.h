#ifndef _WIN_T_SOUND_DEFINITIONS
#define _WIN_T_SOUND_DEFINITIONS

#include "_type/type.predefines.h"
#include "_type/type.callback.h"

enum class _channelType : _u8{
	stream = 0,
	oneshotstatic = 1,
	repeatstatic = 2,
	psg = 3,
	noise = 4
};

enum class _micFormat : _u8{
	bit16 = 0,
	bit8 = 1
};

enum class _channelFormat : _u8{
	none = 3,
	bit8 = 0,	// 8-bit PCM
	bit16 = 1,	// 16-bit PCM
	adpcm = 2,	// IMA ADPCM compressed audio
};

enum class _psgDutyCycle : _u8{
	ignore = 0,
	dc0  = 7,	// 0.0% duty cycle
	dc12 = 0,	// 12.5% duty cycle
	dc25 = 1,	// 25.0% duty cycle
	dc37 = 2,	// 37.5% duty cycle
	dc50 = 3,	// 50.0% duty cycle
	dc62 = 4,	// 62.5% duty cycle
	dc75 = 5,	// 75.0% duty cycle
	dc87 = 6	// 87.5% duty cycle
};

typedef _u32 (_fillingRoutine)( _u32 requestedSize , _s8* , _u16 frequency , _channelFormat format );

struct _fillingParams
{
	_callback<_fillingRoutine>*
					filler;
	_u32			requestedSize;
	_s8*			destination;
	_u16			frequency;
	_channelFormat	format;
};

#endif