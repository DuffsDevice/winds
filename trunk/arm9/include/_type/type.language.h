#ifndef _WIN_T_LANGUAGE_
#define _WIN_T_LANGUAGE_

#include <_type/type.h>
#include <_type/type.shortstring.h>

//! Enumerates available _languages for WinDS
//! Every language is made of two unique characters e.g. "en" for English
using _language = _shortString<3>;

//! Converts between the numbers 0-7 and the supported DS built-in languages in ISO Format
extern _array<_language,8>			id2language;
extern _converter<_language,_u8>	language2id;

#endif