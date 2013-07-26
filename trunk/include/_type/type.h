// Check if already included
#ifndef _WIN_TYPES_
#define _WIN_TYPES_

#include <malloc.h>
#include <list>
#define _GLIBCXX_DEBUG // Because vector::operator[] leads to undefined behaviour when undefined
#include <vector>
#undef _GLIBCXX_DEBUG
#include <array>
#include <string>
#include <map>
using namespace std;

//! Code-sections
#include <nds/memory.h>

//! Undefine if you don't want any speed profiling
//#define DEBUG_PROFILING

//! Overload new and delete
void* operator new(size_t size) __attribute__((malloc));
void operator delete(void *p);
void* operator new[](size_t size) __attribute__((malloc));
void operator delete[](void *p);

template<typename T>
	using _list = std::list<T>;
template<typename T>
	using _vector = std::vector<T>;
template<typename T,typename T2>
	using _map = std::map<T,T2>;
template<typename T,typename T2>
	using _pair = std::pair<T,T2>;
template<typename...T>
	using _tuple = std::tuple<T...>;
template<typename T,unsigned int T2>
	using _array = std::array<T,T2>;
typedef uint8_t 			_u8;
typedef int8_t 				_s8;
typedef uint16_t 			_u16;
typedef int16_t 			_s16;
typedef uint32_t 			_u32;
typedef int32_t 			_s32;
typedef uint64_t 			_u64;
typedef int64_t 			_s64;
typedef char				_char;
typedef _u16 				_length;
typedef _s16 				_coord;
typedef int 				_int;
typedef _u16 				_key;
typedef short unsigned int 	_pixel;
typedef _pixel* 			_pixelArray;
typedef float 				_float;
typedef _u64 				_tempTime;
typedef basic_string<_char>	string;
typedef _vector<string>		_cmdArgs;

#define unused __attribute__(( unused ))
#define noinline __attribute__ ((noinline))
#define DEPRECATED __attribute__((deprecated))

/**
 * Pack two integers into one
 * The union will hopefully be treated
 * as one single _u64/_s64 (faster)
 */
union						_2s32
{
	_s64	val;
	struct{ _s32 first; _s32 second; };
	
	//Ctors
	_2s32() {}
	_2s32( _s64 v ) : val( v ) {}
	_2s32( _s32 f , _s32 s ) : first( f ) , second( s ) {}
	
	operator bool(){ return this->val != 0; }
	_2s32 operator+( _s32 val ){ return _2s32( first + val , second + val ); }
	_2s32 operator-( _s32 val ){ return _2s32( first - val , second - val ); }
	_2s32 operator*( _s32 val ){ return _2s32( first * val , second * val ); }
	_2s32 operator/( _s32 val ){ return _2s32( first / val , second / val ); }
	_2s32& operator+=( _s32 val ){ first += val; second += val; return *this; }
	_2s32& operator-=( _s32 val ){ first -= val; second -= val; return *this; }
	_2s32& operator*=( _s32 val ){ first *= val; second *= val; return *this; }
	_2s32& operator/=( _s32 val ){ first /= val; second /= val; return *this; }
};

static unused _2s32 operator-( const _2s32& val ){ return _2s32( -val.first , -val.second ); }
static unused bool operator!( const _2s32& val ){ return !val.val; }

union 						_2u32
{
	_u64	val;
	struct{ _u32 first; _u32 second; };
	
	// Ctors
	_2u32() {}
	_2u32( _u64 v ) : val( v ) {}
	_2u32( _u32 f , _u32 s ) : first( f ) , second( s ) {}
	
	operator bool(){ return this->val != 0; }
	_2u32 operator+( _s32 val ){ return _2u32( first + val , second + val ); }
	_2u32 operator-( _s32 val ){ return _2u32( first - val , second - val ); }
	_2u32 operator*( _u32 val ){ return _2u32( first * val , second * val ); }
	_2u32 operator/( _u32 val ){ return _2u32( first / val , second / val ); }
	_2u32& operator+=( _s32 val ){ first += val; second += val; return *this; }
	_2u32& operator-=( _s32 val ){ first -= val; second -= val; return *this; }
	_2u32& operator*=( _u32 val ){ first *= val; second *= val; return *this; }
	_2u32& operator/=( _u32 val ){ first /= val; second /= val; return *this; }
};

extern _length SCREEN_WIDTH;
extern _length SCREEN_HEIGHT;

//! Some constraints
template<class T, class B> struct subclass_of {
	static void constraints(T* p) { B* pb = p; }
	subclass_of() { void(*)(T*) = constraints; }
};

template<class T1, class T2> struct copyable {
	static void constraints(T1 a, T2 b) { T2 c = a; b = a; }
	copyable() { void(*)(T1,T2) = constraints; }
};

template<class T1, class T2 = T1> struct comparable {
	static void constraints(T1 a, T2 b) { a==b; a!=b; a<b; a>b; }
	comparable() { void(*)(T1,T2) = constraints; }
};

#define u16 _u16
#include <nds/touch.h>
#undef u16

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

//! Gets the specific component value from a color
static constexpr inline _u8 RGB_GETR( _pixel c ){ return c & 0x1F; }
static constexpr inline _u8 RGB_GETG( _pixel c ){ return ( c >> 5 ) & 0x1F; }
static constexpr inline _u8 RGB_GETB( _pixel c ){ return ( c >> 10 ) & 0x1F; }
static constexpr inline _u8 RGB_GETA( _pixel c ){ return c >> 15; }

//! Sets a specific value
static inline void RGB_SETR( _pixel& c , _u8 red ){ c = ( c & ~0x1F ) | red; }
static inline void RGB_SETG( _pixel& c , _u8 green ){ c = ( c & ~( 0x1F << 5 ) ) | ( green << 5 ); }
static inline void RGB_SETB( _pixel& c , _u8 blue ){ c = ( c & ~( 0x1F << 10 ) ) | ( blue << 10 ); }
static inline void RGB_SETA( _pixel& c , bool alpha ){ c = ( c & ~( 1 << 15 ) ) | ( alpha << 15 ); }

//! Counts, how many digits a number has
extern int countDecimals( _s32 value , _u8 numbersystem = 10 );

//! Reads an int from a string
extern int string2int( const _char * str ) __attribute__(( nonnull (1) ));

//! Converts an int to string
extern string int2string( _int val , _u8 zeroFill = 0 , _u8 numbersystem = 10 );

//! Predefined colors
#define COLOR_TRANSPARENT (_pixel(0))
#define COLOR_YELLOW 	(RGB(31,31,0))
#define COLOR_GREEN 	(RGB(0,31,0))
#define COLOR_CYAN 		(RGB(0,31,31))
#define COLOR_BLUE 		(RGB(0,0,31))
#define COLOR_MAGENTA 	(RGB(31,0,31))
#define COLOR_RED 		(RGB(31,0,0))
#define COLOR_GRAY 		(RGB(15,15,15))
#define COLOR_BLACK 	(_pixel(1<<15))
#define COLOR_WHITE 	(_pixel((1<<16)-1))
#define NO_COLOR 		0


#undef BIT
#undef GETBIT


//! Returns a number with the nth bit set
static constexpr inline _u32 BIT( _u8 n ){ return (_u32)1 << n; }

//! Check if a specific bit in a number is set
static constexpr inline _u8 GETBIT( _u32 value , _u8 bit ){ return ( value >> bit ) & 1; }

//! Bitwise rotation
template<typename T>
static constexpr inline T ROL( T value , _int shift ){ return ( value << shift ) | ( value >> ( sizeof(T) * 8 - shift ) ); }

template<typename T>
static constexpr inline T ROR( T value , _int shift ){ return ( value >> shift ) | ( value << ( sizeof(T) * 8 - shift ) ); }

//! Calculate the Median value of three
_s32 mid( _s32 a , _s32 b , _s32 c );


// Useful things
namespace DSWindows
{
	// The Numbers used here are 
	// ASCII-Values that are not being used
	const _char KEY_A = 1,
	KEY_B = 2,
	KEY_SELECT = 3,
	KEY_START = 4,
	KEY_RIGHT = 5,
	KEY_LEFT	= 6,
	KEY_UP = 7,

	KEY_BACKSPACE = 8,
	KEY_CARRIAGE_RETURN = 10,

	KEY_DOWN	= 14,
	KEY_R = 15,
	KEY_L = 16,
	KEY_X = 17,
	KEY_Y = 18,

	KEY_CAPS	= 19,
	KEY_SHIFT = 20,
	KEY_WINDOWS = 21,
	
	// Excape characters used in fonts to prepare special actions
	STR_CHANGEFONT = 22,
	STR_CHANGEFONTCOLOR = 23,
	STR_CHANGEFONTSIZE = 24;
	
	extern bool isHardwareKey( _char );
	
	//! Displayed as replacemanet if language-specific term
	//! is not available in the currently selected language
	static const char* const emptyStringSignature = "[]";
	
	//! Maps a Libnds Key to a DSWindows-Key
	extern _char libnds2key[12];
};

//! Alignment enumeration
enum class _align : _u8 {
	left,
	center,
	right
};

//! Vertical Alignment enumeration
enum class _valign : _u8 {
	top,
	middle,
	bottom
};

class _padding;
class _margin;

struct _border
{
	_length left;
	_length top;
	_length right;
	_length bottom;
	
	_border( _length l , _length t , _length r , _length b ) : left(l) , top(t) , right(r) , bottom(b) {}
	_border( _length width ) : left(width) , top(width) , right(width) , bottom(width) {}
	_border() : left(0) , top(0) , right(0) , bottom(0) {}
	
	//! Comparisons
	inline bool operator==( const _border& other ) const { return !( *this != other ); }
	inline bool operator!=( const _border& other ) const {
		return other.left != this->left || other.top != this->top || other.right != this->right || other.bottom != this->bottom;
	}
	
	//! Casts for them
	operator _padding();
	operator _margin();
};

//! Typedefs for class _padding and _margin
struct _padding : _border{
	_padding( _length l , _length t , _length r , _length b ) : _border( l , t , r , b ) {}
	_padding( _length width ) : _border( width ) {}
	_padding(){}
};
struct _margin : _border{
	_margin( _length l , _length t , _length r , _length b ) : _border( l , t , r , b ) {}
	_margin( _length width ) : _border( width ) {}
	_margin(){}
};


//! Holds useful information about the touch of the stylus
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

//! Convert _align and _valign to string
extern _map<_align,string> align2string;
extern _map<_valign,string> valign2string;

//! Convert a given string to either _align or _valign
extern _map<string,_align> string2align;
extern _map<string,_valign> string2valign;

//! Enumerates available _languages for WinDS
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

//! Trim not-printable characters at both ends of a given string
void trim( string& , const char* delims = " \n\r\t" , bool front = true , bool back = true );

//! Tools for analyzing the efficiency of code
#ifdef DEBUG_PROFILING
	#include "_type/type.analyzer.h"
#endif

#endif