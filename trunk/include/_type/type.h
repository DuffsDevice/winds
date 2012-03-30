// Check if already included
#ifndef _WIN_TYPES_
#define _WIN_TYPES_

#include <string>
#include <map>

using namespace std;

typedef unsigned int _length;
typedef short int _coord;
typedef unsigned char _char;
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

//! Convert red, green, blue to 15bit Triplette
constexpr _pixel RGB( _u8 r , _u8 g , _u8 b ){
	return r | ( g << 5 ) | ( b << 10 ) | ( 1 << 15 ); // a=1 means opaque, a=0 means transparent
}
constexpr _pixel RGB255( _u8 r , _u8 g , _u8 b ){
	return ( r >> 3 ) | ( ( g >> 3 ) << 5 ) | ( ( b >> 3 ) << 10 ) | ( 1 << 15 ); // a=1 means opaque, a=0 means transparent
}

//! Convert " " " to 16bit Triplette including an alpha-channel
constexpr _pixel RGBA( _u8 r , _u8 g , _u8 b , bool a ){
	return r | ( g << 5 ) | ( b << 10 ) | ( a << 15 ); // a=1 means opaque, a=0 means transparent
}
constexpr _pixel RGBA255( _u8 r , _u8 g , _u8 b , bool a ){
	return ( r >> 3 ) | ( ( g >> 3 ) << 5 ) | ( ( b >> 2 ) << 10 ) | ( ( a >> 3 ) << 15 ); // a=1 means opaque, a=0 means transparent
}
#define RGB_GETR(c) ((c) & 0x1F)
#define RGB_GETG(c) (((c)>>5) & 0x1F)
#define RGB_GETB(c) (((c)>>10) & 0x1F)
#define RGB_GETA(c) (((c)>>15) & 1)
#define NO_COLOR 0

#define DEPRECATED __attribute__((deprecated))


//! Returns a number with the nth bit set
#ifndef BIT
#define BIT(n) ((1) << (n))
#endif

//! Get a specific bit out of a number
#define GETBIT(m,n) (((m) >> (n)) & 1)

#define ROL8(value,shift) ((value) << (shift)) | ((value) >> (8 - (shift)))
#define ROL16(value,shift) ((value) << (shift)) | ((value) >> (16 - (shift)))
#define ROL32(value,shift) ((value) << (shift)) | ((value) >> (32 - (shift)))
#define ROL64(value,shift) ((value) << (shift)) | ((value) >> (64 - (shift)))

#define ROR8(value,shift) ((value) >> (shift)) | ((value) << (8 - (shift)))
#define ROR16(value,shift) ((value) >> (shift)) | ((value) << (16 - (shift)))
#define ROR32(value,shift) ((value) >> (shift)) | ((value) << (32 - (shift)))
#define ROR64(value,shift) ((value) >> (shift)) | ((value) << (64 - (shift)))

//! Calculate the Median value
template <typename name> name mid( name a , name b , name c ){
	name mi = min( a , min( b , c ) );
	name ma = max( a , max( b , c ) );
	if( a == ma ){
		if( b == mi )
			return c;
		return b;
	}
	if( b == ma ){
		if( a == mi )
			return c;
		return a;
	}
	else{
		if( b == mi )
			return a;
		return b;
	}
}

namespace DSWindows
{
	const char KEY_A = 1;
	const char KEY_B = 2;
	const char KEY_SELECT = 3;
	const char KEY_START = 4;
	const char KEY_RIGHT = 5;
	const char KEY_LEFT	= 6;
	const char KEY_UP = 7;

	const char KEY_BACKSPACE = 8;
	const char KEY_CARRIAGE_RETURN = 13;

	const char KEY_DOWN	= 14;
	const char KEY_R = 15;
	const char KEY_L = 16;
	const char KEY_X = 17;
	const char KEY_Y = 18;

	const char KEY_CAPS	= 19;
	const char KEY_SHIFT = 20;
	const char KEY_WINDOWS = 21;
};


// Libnds Keys to Windows-Keys
extern _u8 libnds2key[12];



enum _align{
	left,
	center,
	right,
	optimize
};

enum _valign{
	top,
	middle,
	bottom
};

struct _border{
	_length left;
	_length top;
	_length right;
	_length bottom;
	_border( _length l , _length t , _length r , _length b ) : left(l) , top(t) , right(r) , bottom(b) {}
	_border( _length width ) : left(width) , top(width) , right(width) , bottom(width) {}
	_border() : left(0) , top(0) , right(0) , bottom(0) {}
};

typedef _border _padding;
typedef _border _margin;

extern map<_align,string> align2string;
extern map<_valign,string> valign2string;

extern map<string,_align> string2align;
extern map<string,_valign> string2valign;

/**
 * Specifies the Type of an API-Element
**/
enum _gadgetType{
	button,
	label,
	checkbox,
	textbox,
	keyboard,
	fileview,
	fileobject,
	imagegadget,
	window,
	windows,
	taskboard,
	_plain // No type set
};

#endif