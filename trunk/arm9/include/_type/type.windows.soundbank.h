#ifndef _WIN_T_WINDOWS_SOUND_BANK_
#define _WIN_T_WINDOWS_SOUND_BANK_

#include "_type/type.h"

enum class _windowsSound : _u8
{
	keyClick,
	navigation,
	ping,
	startUp,
	shutDown,
	logOn,
	logOff,
	alert,
	error,
	fatalError,
	minimize,
	restore,
	recycle,
	bubble,
};

class _windowsSoundBank
{
	public:
		
		//! Static class use only!
		_windowsSoundBank() = delete;
		
		static void play( _windowsSound sndType );
};

#endif