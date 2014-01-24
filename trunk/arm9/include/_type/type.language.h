#ifndef _WIN_T_LANGUAGE_
#define _WIN_T_LANGUAGE_

#include "_type/type.h"

//! Enumerates available _languages for WinDS
enum class _language : _u8
{
	japanese = 0,
	english = 1,
	french = 2,
	german = 3,
	italian = 4,
	spanish = 5,
	chinese = 6
};

//! Convert between string and language
extern _fromStr<_language>	string2language;
extern _toStr<_language>	language2string;

#endif