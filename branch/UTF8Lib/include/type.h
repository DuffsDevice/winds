// Check if already included
#ifndef _WIN_TYPES_
#define _WIN_TYPES_

#include <string>

typedef unsigned int _length;
typedef short int _coord;
typedef char _char;
typedef wchar_t _wchar;
typedef uint8_t _u8;
typedef int8_t _s8;
typedef uint16_t _u16;
typedef int16_t _s16;
typedef uint32_t _u32;
typedef int32_t _s32;
typedef uint64_t _u64;
typedef int64_t _s64;
typedef unsigned short int _key;
typedef bool _bit;
typedef short unsigned int _pixel;
typedef _pixel* _pixelArray;
typedef _bit* _bitsMap;
typedef const _char* _literal;
typedef const _wchar* _wliteral;
typedef std::basic_string<_char> string;

#define optimized __attribute__(( hot ))

#endif