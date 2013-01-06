// Check if already included
#ifndef _WIN_TYPES_
#define _WIN_TYPES_

//#include "_type/type.list.h"
#include <malloc.h>
#include <list>
#include <string>
#include <map>
using namespace std;

//! Overload new and delete
void* operator new(size_t size);
void operator delete(void *p);
void* operator new[](size_t size);
void operator delete[](void *p);

typedef unsigned char _char;
template<typename T>
	using _list = std::list<T>;
template<typename T,typename T2>
	using _map = std::map<T,T2>;
typedef uint8_t _u8;
typedef int8_t _s8;
typedef uint16_t _u16;
typedef int16_t _s16;
typedef uint32_t _u32;
typedef int32_t _s32;
typedef uint64_t _u64;
typedef int64_t _s64;
typedef _u32 _length;
typedef _s32 _coord;
typedef int _int;
typedef _u16 _key;
typedef short unsigned int _pixel;
typedef _pixel* _pixelArray;
typedef float _float;
typedef _u32 _tempTime;

extern _length SCREEN_WIDTH;
extern _length SCREEN_HEIGHT;

#define u16 _u16
#include <nds/touch.h>
#undef u16

#define noinline __attribute__ ((noinline))
#define DEPRECATED __attribute__((deprecated))
#define needless __attribute__(( unused ))

typedef basic_string<char> _string;

typedef map<string,string> _cmdArgs;

//! Convert red, green, blue to 15bit Triplette
static constexpr inline _pixel RGB( _u8 r , _u8 g , _u8 b ){
	return r | ( g << 5 ) | ( b << 10 ) | ( 1 << 15 ); // a=1 means opaque, a=0 means transparent
}
static constexpr inline _pixel RGB255( _u8 r , _u8 g , _u8 b ){
	return ( r >> 3 ) | ( ( g >> 3 ) << 5 ) | ( ( b >> 3 ) << 10 ) | ( 1 << 15 ); // a=1 means opaque, a=0 means transparent
}
static constexpr inline _pixel RGBHEX( _u32 hex ){
	return ( ( hex & 0xff0000 ) >> 19 ) | ( ( ( hex & 0x00ff00 ) >> 11 ) << 5 ) | ( ( ( hex & 0x0000ff ) >> 3 ) << 10 ) | ( 1 << 15 ); // a=1 means opaque, a=0 means transparent
}

//! Convert " " " to 16bit Triplette including an alpha-channel
static constexpr inline _pixel RGBA( _u8 r , _u8 g , _u8 b , bool a ){
	return r | ( g << 5 ) | ( b << 10 ) | ( a << 15 ); // a=1 means opaque, a=0 means transparent
}
static constexpr inline _pixel RGBA255( _u8 r , _u8 g , _u8 b , bool a ){
	return ( r >> 3 ) | ( ( g >> 3 ) << 5 ) | ( ( b >> 2 ) << 10 ) | ( ( a >> 3 ) << 15 ); // a=1 means opaque, a=0 means transparent
}
static constexpr inline _pixel RGBAHEX( _u32 hex , bool alpha ){
	return ( ( hex & 0xff0000 ) >> 19 ) | ( ( ( hex & 0x00ff00 ) >> 11 ) << 5 ) | ( ( ( hex & 0x0000ff ) >> 3 ) << 10 ) | ( alpha << 15 ); // a=1 means opaque, a=0 means transparent
}

static constexpr inline _u8 RGB_GETR( _pixel c ){
	return c & 0x1F;
}

static constexpr inline _u8 RGB_GETG( _pixel c ){
	return ( c >> 5 ) & 0x1F;
}

static constexpr inline _u8 RGB_GETB( _pixel c ){
	return ( c >> 10 ) & 0x1F;
}

static constexpr inline _u8 RGB_GETA( _pixel c ){
	return ( c >> 15 ) & 1;
}

static needless int getDecimals( _s32 value )
{
	int i = 0;
	while( value > 0 )
	{
		i++;
		value /= 10;
	}
	return i;
}

extern int string2int( const char * );
extern string int2string( _int val , _u8 zeroFill = 0 );

#define COLOR_TRANSPARENT (_pixel(0))
#define COLOR_YELLOW 	(RGB(31,31,0))
#define COLOR_GREEN 	(RGB(0,31,0))
#define COLOR_CYAN 		(RGB(0,31,31))
#define COLOR_BLUE 		(RGB(0,0,31))
#define COLOR_MAGENTA 	(RGB(31,0,31))
#define COLOR_RED 		(RGB(31,0,0))
#define COLOR_BLACK 	(_pixel(1<<15))
#define COLOR_WHITE 	(_pixel((1<<16)-1))
#define NO_COLOR 0


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
_s32 mid( _s32 a , _s32 b , _s32 c );

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
	
	extern bool isHardwareKey( _char );
};


// Libnds Keys to Windows-Keys
extern _char libnds2key[12];

enum class _align : _u8 {
	left,
	center,
	right
};

enum class _valign : _u8 {
	top,
	middle,
	bottom
};

class _border
{
	private:
		_length left;
		_length top;
		_length right;
		_length bottom;
		
	public:
		_border( _length l , _length t , _length r , _length b ) : left(l) , top(t) , right(r) , bottom(b) {}
		_border( _length width ) : left(width) , top(width) , right(width) , bottom(width) {}
		_border() : left(0) , top(0) , right(0) , bottom(0) {}
		
		_length getLeft() const { return left; }
		_length getRight() const { return right; }
		_length getTop() const { return top; }
		_length getBottom() const { return bottom; }
		
		void setLeft( _length val ){ left = val; }
		void setRight( _length val ){ right = val; }
		void setTop( _length val ){ top = val; }
		void setBottom( _length val ){ bottom = val; }
		
		bool operator==( const _border& other )
		{
			return !( other.left != this->left || other.top != this->top || other.right != this->right || other.bottom != this->bottom );
		}
		
		bool operator!=( const _border& other )
		{
			return other.left != this->left || other.top != this->top || other.right != this->right || other.bottom != this->bottom;
		}
};

typedef _border _padding;
typedef _border _margin;

struct _touch
{
	_coord x;
	_coord y;
	
	_touch& operator=( touchPosition t )
	{
		x = t.px;
		y = t.py;
		return *this;
	}
	
	_touch( touchPosition t ){ *this = t; }
	
	_touch() :
		x( 0 )
		, y ( 0 )
	{ }
};

extern map<_align,string> align2string;
extern map<_valign,string> valign2string;

extern map<string,_align> string2align;
extern map<string,_valign> string2valign;


/**
 * Specifies the Type of an API-Element
**/
enum class _gadgetType : _u8
{
	button,
	label,
	checkbox,
	radiobox,
	textbox,
	counter,
	selectbox,
	selectitem,
	progressbar,
	keyboard,
	desktop,
	fileobject,
	imagegadget,
	scrollarea,
	scrollbutton,
	scrollbar,
	window,
	screen,
	contextmenu,
	_plain // No type set (is probably not used)
};

enum class _language : _u8
{
	japanese = 0,
	english,
	french,
	german,
	italian,
	spanish,
	chinese
};

void trim( string& );

extern map<_gadgetType,string> gadgetType2string;

#endif

#include "_type/type.analyzer.h"