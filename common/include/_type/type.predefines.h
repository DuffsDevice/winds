// Check if already included
#ifndef _WIN_T_PREDEFINES_
#define _WIN_T_PREDEFINES_

#define _WIN_VER_MAJOR_ 0
#define _WIN_VER_MINOR_ 98
#define _WIN_VER_STATE_ "beta"

#include <nds/ndstypes.h>
#include <stdint.h>
#include <stdbool.h>

//! Additional CV-Like Qualifiers
#define noinline __attribute__(( noinline ))
#define forceinline __attribute__(( always_inline )) inline
#define optimized __attribute__(( hot ))

typedef uint8_t 					_u8;
typedef int8_t 						_s8;
typedef uint16_t 					_u16;
typedef int16_t 					_s16;
typedef unsigned int				_u32;
typedef int		 					_s32; // int32_t = long int = int (preserves backwards compatibility)
typedef uint64_t 					_u64;
typedef int64_t 					_s64;
typedef bool						_bool;
typedef _u8							_byte;
typedef _s16 						_length;
typedef _s16 						_coord;
typedef char						_char;
typedef int 						_int;
typedef _u16			 			_pixel;
typedef _pixel* 					_pixelArray;
typedef float 						_float;
typedef _u64 						_tempTime;
typedef const _char*				_literal;
typedef int							_unixTime;

#include <nds/ndstypes.h>
#include <nds/touch.h>
#undef BIT
#undef GETBIT

//! Returns a number with the nth bit set
static constexpr inline _u32 BIT( _u8 n ){ return (_u32)1 << n; }

//! Check if a specific bit in a number is set
static constexpr inline bool GETBIT( _u32 value , _u8 bit ){ return ( value >> bit ) & 1; }


//! Bitwise rotation
template<typename T>
static constexpr inline T ROL( T value , _int shift ){ return ( value << shift ) | ( value >> ( sizeof(T) * 8 - shift ) ); }
template<typename T>
static constexpr inline T ROR( T value , _int shift ){ return ( value >> shift ) | ( value << ( sizeof(T) * 8 - shift ) ); }

#endif